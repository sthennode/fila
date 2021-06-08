///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2020 $organization$
///
/// This software is provided by the author and contributors ``as is'' 
/// and any express or implied warranties, including, but not limited to, 
/// the implied warranties of merchantability and fitness for a particular 
/// purpose are disclaimed. In no event shall the author or contributors 
/// be liable for any direct, indirect, incidental, special, exemplary, 
/// or consequential damages (including, but not limited to, procurement 
/// of substitute goods or services; loss of use, data, or profits; or 
/// business interruption) however caused and on any theory of liability, 
/// whether in contract, strict liability, or tort (including negligence 
/// or otherwise) arising in any way out of the use of this software, 
/// even if advised of the possibility of such damage.
///
///   File: semaphore.hpp
///
/// Author: $author$
///   Date: 1/7/2020
///////////////////////////////////////////////////////////////////////
#if !defined(XOS_MT_POSIX_SEMAPHORE_HPP) || defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
#if !defined(XOS_MT_POSIX_SEMAPHORE_HPP) && !defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
#define XOS_MT_POSIX_SEMAPHORE_HPP
#endif /// !defined(XOS_MT_POSIX_SEMAPHORE_HPP) && !defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)

#include "xos/mt/semaphore.hpp"
#include "xos/mt/posix/timed.hpp"

#if defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
#define PLATFORM_OS_POSIX_SEMAPHORE
#include "xos/platform/os/posix/semaphore.h"
#else defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
#if defined(WINDOWS)
#include "xos/platform/microsoft/windows/posix/semaphore.h"
#else /// defined(WINDOWS)
#if defined(APPLEOSX)
#if !defined(__MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM)
#define __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM __MAC_10_9
#endif /// !defined(__MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// defined(APPLEOSX) 
#endif /// defined(APPLEOSX) 

#if !defined(HAS_POSIX_SEMAPHORE)
#if !defined(WINDOWS)
#if defined(APPLEOSX)
#if (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#define NO_HAS_POSIX_SEMAPHORE
#endif /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// defined(APPLEOSX)
#define HAS_POSIX_SEMAPHORE
#endif /// defined(APPLEOSX)
#endif /// !defined(WINDOWS)
#endif /// !defined(HAS_POSIX_SEMAPHORE)

#if defined(HAS_POSIX_SEMAPHORE)
#include <semaphore.h>
#include <errno.h>
#else /// defined(HAS_POSIX_SEMAPHORE)
#include "xos/platform/os/posix/semaphore.h"
#endif /// defined(HAS_POSIX_SEMAPHORE)
#endif /// defined(WINDOWS)
#endif /// defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#if defined(SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// defined(SEM_HAS_TIMEDWAIT)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#if defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

///
/// sem_timedwait
/// 
#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
inline int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    if ((sem) && (abs_timeout)) {
        int err = EFAULT;
        struct timespec abs_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &abs_time))) {
            useconds_t time_useconds = ::xos::seconds_useconds(abs_time.tv_sec) + ::xos::nseconds_useconds(abs_time.tv_nsec),
                       timeout_useconds = ::xos::seconds_useconds(abs_timeout->tv_sec) + ::xos::nseconds_useconds(abs_timeout->tv_nsec);
            if (timeout_useconds > time_useconds) {
                int err = 0;            
                for (useconds_t useconds_sleep = ::xos::mseconds_useconds(10);
                     timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
                    if (EAGAIN != (err = ::sem_trywait(sem))) {
                        if (EBUSY != (err)) {
                            if (ETIMEDOUT != (err)) {
                                return err;
                            }
                        }
                    }
                    ::usleep(useconds_sleep);
                }
            }
            return ETIMEDOUT;
        }
    }
    return EINVAL;
}
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

///
/// sem_timedwait_relative_np
/// 
#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
inline int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
#if defined(POSIX_SEM_HAS_TIMEDWAIT)
#if defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
    if ((sem) && (timeout)) {
        int err = EFAULT;
        struct timespec until_time;
        if (!(err = ::clock_gettime_relative_np(&until_time, timeout))) {
            if ((err = ::sem_timedwait(sem, &until_time))) {
                err = errno;
            }
        }
        return err;
    }
#endif /// defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT)
    return EINVAL;
}
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

namespace xos {
namespace mt {
#if defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
namespace os {
#endif /// defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
namespace posix {

/// enum sem_pshared_t
typedef int sem_pshared_t;
enum {
    SEMAPHORE_PROCESS_PRIVATE = 0,
    SEMAPHORE_PROCESS_SHARED  = 1
}; /// enum sem_pshared_t

/// class semaphoret
template 
<typename TWhat = sem_t,
 class TExtends = mt::extended::semaphoret<TWhat*>, 
 class TImplements = typename TExtends::implements>
class exported semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef semaphoret derives;
    
    typedef TWhat semaphore_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    semaphoret(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    semaphoret(const semaphoret& copy): extends(copy) {
    }
    semaphoret() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~semaphoret() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// ...acquire... / ...release...
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_acquire_detached(detached, milliseconds);
        }
        return acquire_failed;
    }
    virtual acquire_status untimed_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual acquire_status try_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual bool release() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return release_detached(detached);
        }
        return false;
    }

    /// ...acquire_detached... / ...release_detached...
    virtual bool acquire_detached(attached_t detached) const { 
        return (acquire_success == (untimed_acquire_detached(detached))); 
    }
    virtual acquire_status timed_acquire_detached(attached_t detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            int err = 0;
            struct timespec until_time;
            until_time.tv_sec =  mseconds_seconds(milliseconds);
            until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::sem_timedwait_relative_np(detached, &until_time)...");
            if (!(err = ::sem_timedwait_relative_np(detached, &until_time))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::sem_timedwait_relative_np(detached, &until_time)");
                return acquire_success;
            } else {
                err = errno;
                if (EBUSY == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...EBUSY err = "<< err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                    return acquire_busy;
                } else {
                    if (ETIMEDOUT == (err)) {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...ETIMEOUT err = "<< err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                        return acquire_timeout;
                    } else {
                        if (EINTR == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                            return acquire_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sem_timedwait_relative_np(detached, &until_time)");
                        }
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(detached);
            } else {
                return try_acquire_detached(detached);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            LOGGER_IS_LOGGED_TRACE("::sem_trywait(detached)...");
            if (!(err = ::sem_trywait(detached))) {
                LOGGER_IS_LOGGED_TRACE("...::sem_trywait(detached)");
                return acquire_success;
            } else {
                err = errno;
                if (EAGAIN == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...EAGAIN err = "<< err << " on ::sem_trywait(detached)");
                    return acquire_busy;
                } else {
                    if (EBUSY == (err)) {
                        LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::sem_trywait(detached)");
                        return acquire_busy;
                    } else {
                        if (ETIMEDOUT == (err)) {
                            LOGGER_IS_LOGGED_TRACE("...ETIMEOUT err = "<< err << " on ::sem_trywait(detached)");
                            return acquire_timeout;
                        } else {
                            if (EINTR == (err)) {
                                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sem_trywait(detached)");
                                return acquire_interrupted;
                            } else {
                                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sem_trywait(detached)");
                            }
                        }
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::sem_wait(detached)...");
            if (!(err = ::sem_wait(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::sem_wait(detached)");
                return acquire_success;
            } else {
                err = errno;
                if (EBUSY == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...EBUSY err = "<< err << " on ::sem_wait(detached)");
                    return acquire_busy;
                } else {
                    if (ETIMEDOUT == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...ETIMEOUT err = "<< err << " on ::sem_wait(detached)");
                        return acquire_timeout;
                    } else {
                        if (EINTR == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sem_wait(detached)");
                            return acquire_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sem_wait(detached)");
                        }
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::sem_post(detached)...");
            if (!(err = ::sem_post(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::sem_post(detached)");
                return true;
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sem_post(detached)");
            }
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(semaphore_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached(sem_t& sem) const {
        size_t initially_released = 0;
        return create_detached(sem, initially_released);
    }
    virtual attached_t create_detached(sem_t& sem, size_t initially_released) const {
        sem_pshared_t pshared = SEMAPHORE_PROCESS_PRIVATE;
        return create_detached(sem, pshared, initially_released);
    }
    virtual attached_t create_detached(sem_t& sem, sem_pshared_t pshared, size_t initially_released) const {
        attached_t detached = (attached_t)(unattached);
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::sem_init(&sem, pshared, initially_released)...");
        if (!(err = ::sem_init(&sem, pshared, initially_released))) {
            LOGGER_IS_LOGGED_DEBUG("...::sem_init(&sem, pshared, initially_released)");
            detached = &sem;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::sem_init(&sem, pshared, initially_released)...");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::sem_destroy(detached)...");
            if (!(err = ::sem_destroy(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::sem_destroy(detached)");
                return true;
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sem_destroy(detached)");
            }
        }
        return false;
    }
    
protected:
    semaphore_t semaphore_;
}; /// class semaphoret
typedef semaphoret<> semaphore;

} /// namespace posix
#if defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
} /// namespace os 
#endif /// defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
} /// namespace mt
} /// namespace xos

#endif /// !defined(XOS_MT_POSIX_SEMAPHORE_HPP) || defined(XOS_MT_OS_POSIX_SEMAPHORE_HPP)
