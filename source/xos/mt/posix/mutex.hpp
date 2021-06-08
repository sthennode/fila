///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2019 $organization$
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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 12/25/2019
///////////////////////////////////////////////////////////////////////
#if !defined(XOS_MT_POSIX_MUTEX_HPP) || defined(XOS_MT_OS_POSIX_MUTEX_HPP)
#if !defined(XOS_MT_POSIX_MUTEX_HPP) && !defined(XOS_MT_OS_POSIX_MUTEX_HPP)
#define XOS_MT_POSIX_MUTEX_HPP
#endif /// !defined(XOS_MT_POSIX_MUTEX_HPP) && !defined(XOS_MT_OS_POSIX_MUTEX_HPP)

#include "xos/mt/mutex.hpp"
#include "xos/mt/posix/timed.hpp"

#if defined(XOS_MT_OS_POSIX_MUTEX_HPP)
#include "xos/platform/os/posix/mutex.h"
#else defined(XOS_MT_OS_POSIX_MUTEX_HPP)
#if !defined(WINDOWS)
#include <pthread.h>
#include <time.h>
#include <errno.h>
#else /// !defined(WINDOWS)
#include "xos/platform/microsoft/windows/posix/mutex.h"
#endif /// !defined(WINDOWS)
#endif /// defined(XOS_MT_OS_POSIX_MUTEX_HPP)

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#endif /// defined(HAS_POSIX_TIMEOUTS)

///
/// pthread_mutex_timedlock
/// 
#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
inline int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout) {
    if ((mutex) && (abs_timeout)) {
        int err = 0; 
        struct timespec abs_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &abs_time))) {
            useconds_t time_useconds = ::xos::seconds_useconds(abs_time.tv_sec) + ::xos::nseconds_useconds(abs_time.tv_nsec),
                       timeout_useconds = ::xos::seconds_useconds(abs_timeout->tv_sec) + ::xos::nseconds_useconds(abs_timeout->tv_nsec);
            if (timeout_useconds > time_useconds) {
                int err = 0;            
                for (useconds_t useconds_sleep = ::xos::mseconds_useconds(10);
                     timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
                    if (EBUSY != (err = ::pthread_mutex_trylock(mutex))) {
                        return err;
                    }
                    ::usleep(useconds_sleep);
                }
            }
            return ETIMEDOUT;
        }
    }
    return EINVAL; 
}
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)

///
/// pthread_mutex_timedlock_relative_np
/// 
#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
inline int pthread_mutex_timedlock_relative_np(pthread_mutex_t *mutex, const struct timespec *timeout) {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#if defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
    if ((mutex) && (timeout)) {
        int err = EFAULT; 
        struct timespec until_time;
        if (!(err = ::clock_gettime_relative_np(&until_time, timeout))) {
            if (0 > (err = ::pthread_mutex_timedlock(mutex, &until_time))) {
                err = errno;
            }
        }
        return err;
    }
#endif /// defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
    return EINVAL;
}
#define PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)

namespace xos {
namespace mt {
#if defined(XOS_MT_OS_POSIX_MUTEX_HPP)
namespace os {
#endif /// defined(XOS_MT_OS_POSIX_MUTEX_HPP)
namespace posix {

typedef pthread_mutexattr_t mutexattr_t;
typedef pthread_mutex_t mutex_t;
typedef mutex_t* mutex_attached_t;

/// class mutext
template 
<class TExtends = mt::extended::mutext<mutex_attached_t>, 
 class TImplements = typename TExtends::implements>
class exported mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef mutext derives;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    mutext(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    mutext(const mutext& copy): extends(copy) {
    }
    mutext() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~mutext() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// create / destroy
    virtual bool create(bool initially_locked) {
        if ((this->destroyed())) {
            attached_t detached = 0;
            if ((detached = this->create_detached(mutex_, initially_locked))) {
                this->attach_created(detached);
                return true;
            }
        }
        return false;
    }
    virtual bool create() {
        return create(false);
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            if ((this->destroy_detached(*detached))) {
                return true;
            }
        }
        return false;
    }

    /// ...lock / ...unlock
    virtual lock_status time_lock(mseconds_t milliseconds) {
        lock_status status = lock_failed;
        if (0 < milliseconds) {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            attached_t detached = 0;
            if ((detached = this->attached_to())) {
                int err = 0;
                struct timespec until_time;

                until_time.tv_sec =  mseconds_seconds(milliseconds);
                until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

                LOGGER_IS_LOGGED_TRACE("::pthread_mutex_timedlock(detached, &until_time)...");
                if (!(err = ::pthread_mutex_timedlock(detached, &until_time))) {
                    LOGGER_IS_LOGGED_TRACE("...::pthread_mutex_timedlock(detached, &until_time)");
                    return lock_success;
                } else {
                    if (EBUSY != (err)) {
                        if (ETIMEDOUT != (err)) {
                            if (EINTR != (err)) {
                                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_timedlock(detached, &until_time)");
                            } else {
                                LOGGER_IS_LOGGED_ERROR("...failed EINTR err = " << err << " on ::pthread_mutex_timedlock(detached, &until_time)");
                                return lock_interrupted;
                            }
                        } else {
                            LOGGER_IS_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on ::pthread_mutex_timedlock(detached, &until_time)");
                            return lock_busy;
                        }
                    } else {
                        LOGGER_IS_LOGGED_TRACE("...failed EBUSY err = " << err << " on ::pthread_mutex_timedlock(detached, &until_time)");
                        return lock_busy;
                    }
                }
            }
#else /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            LOGGER_IS_LOGGED_ERROR("...invalid ::pthread_mutex_timedlock(detached, ...)");
            status = lock_invalid;
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
        }
        return status;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        lock_status status = lock_failed;
        if (0 > milliseconds) {
            status = untimed_lock();
        } else {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
            attached_t detached = 0;
            if ((detached = this->attached_to())) {
                int err = 0;
                bool logged = this->is_logged(), 
                     debug = this->is_logged_debug(milliseconds);
                struct timespec until_time;

                until_time.tv_sec =  mseconds_seconds(milliseconds);
                until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

                LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "::pthread_mutex_timedlock_relative_np(detached, &until_time)...");
                if (!(err = ::pthread_mutex_timedlock_relative_np(detached, &until_time))) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...::pthread_mutex_timedlock_relative_np(detached, &until_time)");
                    return lock_success;
                } else {
                    if (EBUSY != (err)) {
                        if (ETIMEDOUT != (err)) {
                            if (EINTR != (err)) {
                                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_timedlock_relative_np(detached, &until_time)");
                            } else {
                                LOGGER_IS_LOGGED_ERROR("...failed EINTR err = " << err << " on ::pthread_mutex_timedlock_relative_np(detached, &until_time)");
                                return lock_interrupted;
                            }
                        } else {
                            LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...failed ETIMEDOUT err = " << err << " on ::pthread_mutex_timedlock_relative_np(detached, &until_time)");
                            return lock_busy;
                        }
                    } else {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...failed EBUSY err = " << err << " on ::pthread_mutex_timedlock_relative_np(detached, &until_time)");
                        return lock_busy;
                    }
                }
            }
#else /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
            if (milliseconds) {
                LOGGER_IS_LOGGED_ERROR("...invalid ::pthread_mutex_timedlock_relative_np(detached, ...)");
                status = lock_invalid;
            } else {
                status = this->try_lock();
            }
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
        }
        return status;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            LOGGER_IS_LOGGED_TRACE("::pthread_mutex_trylock(detached)...");
            if (!(err = ::pthread_mutex_trylock(detached))) {
                LOGGER_IS_LOGGED_TRACE("...::pthread_mutex_trylock(detached)");
                return lock_success;
            } else {
                if (EBUSY != (err)) {
                    if (ETIMEDOUT != (err)) {
                        if (EINTR != (err)) {
                            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_trylock(detached)");
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed EINTR err = " << err << " on ::pthread_mutex_trylock(detached)");
                            return lock_interrupted;
                        }
                    } else {
                        LOGGER_IS_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on ::pthread_mutex_trylock(detached)");
                        return lock_busy;
                    }
                } else {
                    LOGGER_IS_LOGGED_TRACE("...failed EBUSY err = " << err << " on ::pthread_mutex_trylock(detached)");
                    return lock_busy;
                }
            }
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::pthread_mutex_lock(detached)...");
            if (!(err = ::pthread_mutex_lock(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::pthread_mutex_lock(detached)");
                return lock_success;
            } else {
                if (EINTR != (err)) {
                    if (EBUSY != (err)) {
                        if (ETIMEDOUT != (err)) {
                            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_lock(detached)");
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed ETIMEDOUT err = " << err << " on ::pthread_mutex_lock(detached)");
                            return lock_timeout;
                        }
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed EBUSY err = " << err << " on ::pthread_mutex_lock(detached)");
                        return lock_busy;
                    }
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed EINTR err = " << err << " on ::pthread_mutex_lock(detached)");
                    return lock_interrupted;
                }
            }
        }
        return lock_failed;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::pthread_mutex_unlock(detached)...");
            if (!(err = ::pthread_mutex_unlock(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::pthread_mutex_unlock(detached)");
                return true;
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err =" << err << " on ::pthread_mutex_unlock(detached)");
            }
        }
        return false;
    }

    /// create... / destroy...
    virtual attached_t create_attached(mutex_t& mutex, bool initially_locked) {
        attached_t detached = 0;
        if ((detached = this->create_detached(mutex_, initially_locked))) {
            this->attach_created(detached);
            return detached;
        }
        return 0;
    }
    virtual attached_t create_detached(mutex_t& mutex, bool initially_locked) const {
        int err = 0;
        mutexattr_t mutexattr;
        LOGGER_IS_LOGGED_DEBUG("::pthread_mutexattr_init(&mutexattr)...");
        if (!(err = ::pthread_mutexattr_init(&mutexattr))) {
            LOGGER_IS_LOGGED_DEBUG("::pthread_mutex_init(&mutex, &mutexattr)...");
            if (!(err = ::pthread_mutex_init(&mutex, &mutexattr))) {
                LOGGER_IS_LOGGED_DEBUG("::pthread_mutexattr_destroy(&mutexattr)...");
                if (!(err = ::pthread_mutexattr_destroy(&mutexattr))) {
                    return &mutex;
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed err =" << err << " on ::pthread_mutexattr_destroy(&mutexattr)");
                    LOGGER_IS_LOGGED_DEBUG("::pthread_mutex_destroy(&mutex)...");
                    if ((err = ::pthread_mutex_destroy(&mutex))) {
                        LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_destroy(&mutex)");
                    }
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_init(&mutex, &mutexattr)");
                LOGGER_IS_LOGGED_DEBUG("::pthread_mutexattr_destroy(&mutexattr)...");
                if ((err = ::pthread_mutexattr_destroy(&mutexattr))) {
                    LOGGER_IS_LOGGED_ERROR("...failed err =" << err << " on ::pthread_mutexattr_destroy(&mutexattr)");
                }
            }
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err =" << err << " on ::pthread_mutexattr_init(&mutexattr)");
        }
        return 0;
    }
    virtual bool destroy_detached(mutex_t& mutex) const {
        bool success = true;
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::pthread_mutex_destroy(&mutex)...");
        if ((err = ::pthread_mutex_destroy(&mutex))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_mutex_destroy(&mutex)");
            success = false;
        }
        return success;
    }

protected:
    mutex_t mutex_;
}; /// class mutext
typedef mutext<> mutex;

} /// namespace posix
#if defined(XOS_MT_OS_POSIX_MUTEX_HPP)
} /// namespace os 
#endif /// defined(XOS_MT_OS_POSIX_MUTEX_HPP)
} /// namespace mt
} /// namespace xos

#endif /// !defined(XOS_MT_POSIX_MUTEX_HPP) || defined(XOS_MT_OS_POSIX_MUTEX_HPP)
