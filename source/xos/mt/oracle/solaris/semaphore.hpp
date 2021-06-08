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
///   Date: 1/9/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HPP
#define XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HPP

#include "xos/mt/semaphore.hpp"

#if defined(SOLARIS)
#include <synch.h>
#else /// defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/semaphore.h"
#endif /// defined(SOLARIS)

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

/// class semaphoret
template 
<class TExtends = mt::extended::semaphoret<sema_t*>, 
 class TImplements = typename TExtends::implements>
class exported semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef semaphoret derives;
    
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
    virtual acquire_status time_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return time_acquire_detached(*detached, milliseconds);
        }
        return acquire_failed;
    }
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_acquire_detached(*detached, milliseconds);
        }
        return acquire_failed;
    }
    virtual acquire_status untimed_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_acquire_detached(*detached);
        }
        return acquire_failed;
    }
    virtual acquire_status try_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_acquire_detached(*detached);
        }
        return acquire_failed;
    }
    virtual bool release() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return release_detached(*detached);
        }
        return false;
    }

    /// ...acquire_detached... / ...release_detached...
    virtual bool acquire_detached(sema_t& sem) const { 
        return (acquire_success == (untimed_acquire_detached(sem))); 
    }
    virtual acquire_status timed_acquire_detached(sema_t& sem, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            int err = 0;
            timestruc_t until_time;
            until_time.tv_sec =  mseconds_seconds(milliseconds);
            until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::sema_reltimedwait(&sem, &until_time)...");
            if (!(err = ::sema_reltimedwait(&sem, &until_time))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::sema_reltimedwait(&sem, &until_time)");
                return acquire_success;
            } else {
                if (EBUSY == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...EBUSY err = "<< err << " on ::sema_reltimedwait(&sem, &until_time)");
                    return acquire_busy;
                } else {
                    if (ETIMEDOUT == (err)) {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...ETIMEOUT err = "<< err << " on ::sema_reltimedwait(&sem, &until_time)");
                        return acquire_timeout;
                    } else {
                        if (EINTR == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sema_reltimedwait(&sem, &until_time)");
                            return acquire_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_reltimedwait(&sem, &until_time)");
                        }
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(sem);
            } else {
                return try_acquire_detached(sem);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status time_acquire_detached(sema_t& sem, mseconds_t milliseconds) const { 
        int err = 0;
        timestruc_t until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IS_LOGGED_TRACE("::sema_timedwait(&sem, &until_time)...");
        if (!(err = ::sema_timedwait(&sem, &until_time))) {
            LOGGER_IS_LOGGED_TRACE("...::sema_timedwait(&sem, &until_time)");
            return acquire_success;
        } else {
            if (EBUSY == (err)) {
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::sema_timedwait(&sem, &until_time)");
                return acquire_busy;
            } else {
                if (ETIMEDOUT == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...ETIMEOUT err = "<< err << " on ::sema_timedwait(&sem, &until_time)");
                    return acquire_timeout;
                } else {
                    if (EINTR == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sema_timedwait(&sem, &until_time)");
                        return acquire_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_timedwait(&sem, &until_time)");
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(sema_t& sem) const { 
        int err = 0;
        LOGGER_IS_LOGGED_TRACE("::sema_trywait(&sem)...");
        if (!(err = ::sema_trywait(&sem))) {
            LOGGER_IS_LOGGED_TRACE("...::sema_trywait(&sem)");
            return acquire_success;
        } else {
            if (EBUSY == (err)) {
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::sema_trywait(&sem)");
                return acquire_busy;
            } else {
                if (ETIMEDOUT == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...ETIMEOUT err = "<< err << " on ::sema_trywait(&sem)");
                    return acquire_timeout;
                } else {
                    if (EINTR == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sema_trywait(&sem)");
                        return acquire_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_trywait(&sem)");
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(sema_t& sem) const { 
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::sema_wait(&sem)...");
        if (!(err = ::sema_wait(&sem))) {
            LOGGER_IS_LOGGED_DEBUG("...::sema_wait(&sem)");
            return acquire_success;
        } else {
            if (EBUSY == (err)) {
                LOGGER_IS_LOGGED_ERROR("...EBUSY err = "<< err << " on ::sema_wait(&sem)");
                return acquire_busy;
            } else {
                if (ETIMEDOUT == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...ETIMEOUT err = "<< err << " on ::sema_wait(&sem)");
                    return acquire_timeout;
                } else {
                    if (EINTR == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::sema_wait(&sem)");
                        return acquire_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_wait(&sem)");
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(sema_t& sem) const { 
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::sema_post(&sem)...");
        if (!(err = ::sema_post(&sem))) {
            LOGGER_IS_LOGGED_DEBUG("...::sema_post(&sem)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_post(&sem)");
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(sema_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            return destroy_detached(*detached);
        }
        return false;
    }
    virtual attached_t create_detached(sema_t& sem) const {
        size_t initially_released = 0;
        return create_detached(sem, initially_released);
    }
    virtual attached_t create_detached(sema_t& sem, size_t initially_released) const {
        int type = USYNC_THREAD;
        return create_detached(sem, type, initially_released);
    }
    virtual attached_t create_detached(sema_t& sem, int type, size_t initially_released) const {
        attached_t detached = (attached_t)(unattached);
        void *arg = 0;
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::sema_init(&sem, initially_released, type, arg)...");
        if (!(err = ::sema_init(&sem, initially_released, type, arg))) {
            LOGGER_IS_LOGGED_DEBUG("...::sema_init(&sem, initially_released, type, arg)");
            detached = &sem;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::sema_init(&sem, initially_released, type, arg)...");
        }
        return detached;
    }
    virtual bool destroy_detached(sema_t& sem) const {
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::sema_destroy(&sem)...");
        if (!(err = ::sema_destroy(&sem))) {
            LOGGER_IS_LOGGED_DEBUG("...::sema_destroy(&sem)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::sema_destroy(&sem)");
        }
        return false;
    }

protected:
    sema_t sema_;
}; /// class semaphoret
typedef semaphoret<> semaphore;

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HPP 
