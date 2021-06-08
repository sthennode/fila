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
///   Date: 12/30/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_ORACLE_SOLARIS_MUTEX_HPP
#define XOS_MT_ORACLE_SOLARIS_MUTEX_HPP

#include "xos/mt/mutex.hpp"
#include "xos/base/time.hpp"

#if defined(SOLARIS)
#include <thread.h> 
#include <synch.h>
#else /// defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/mutex.h"
#endif /// defined(SOLARIS)

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

/// class mutext
template 
<class TExtends = mt::extended::mutext<mutex_t*>, 
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

    /// ...lock... / ...unlock...
    virtual lock_status time_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return time_lock_detached(*detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_lock_detached(*detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_lock_detached(*detached);
        }
        return lock_failed;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_lock_detached(*detached);
        }
        return lock_failed;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return unlock_detached(*detached);
        }
        return false;
    }

    /// ...lock... / ...unlock...
    virtual bool lock_detached(mutex_t& mutex) const { 
        return (lock_success == (untimed_lock_detached(mutex))); 
    }
    virtual lock_status timed_lock_detached(mutex_t& mutex, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            int err = 0;
            timestruc_t until_time;
            until_time.tv_sec =  mseconds_seconds(milliseconds);
            until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::mutex_reltimedlock(&mutex, &until_time)...");
            if ((err = ::mutex_reltimedlock(&mutex, &until_time))) {
                switch(err) {
                case EBUSY:
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...EBUSY err = "<< err << " on ::mutex_reltimedlock(&mutex, &until_time)");
                    return lock_busy;
                case ETIMEDOUT:
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...ETIMEDOUT err = "<< err << " on ::mutex_reltimedlock(&mutex, &until_time)");
                    return lock_timeout;
                case EINTR:
                    LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::mutex_reltimedlock(&mutex, &until_time)");
                    return lock_interrupted;
                default:
                    LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_reltimedlock(&mutex, &until_time)");
                    return lock_failed;
                }
            } else {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::mutex_reltimedlock(&mutex, &until_time)");
                return lock_success;
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_lock_detached(mutex);
            } else {
                return try_lock_detached(mutex);
            }
        }
        return lock_failed; 
    }
    virtual lock_status time_lock_detached(mutex_t& mutex, mseconds_t milliseconds) const { 
        int err = 0;
        timestruc_t until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IS_LOGGED_TRACE("::mutex_timedlock(&mutex, &until_time)...");
        if ((err = ::mutex_timedlock(&mutex, &until_time))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::mutex_timedlock(&mutex, &until_time)");
                return lock_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::mutex_timedlock(&mutex, &until_time)");
                return lock_timeout;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::mutex_timedlock(&mutex, &until_time)");
                return lock_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_timedlock(&mutex, &until_time)");
                return lock_failed;
            }
        } else {
            LOGGER_IS_LOGGED_TRACE("...::mutex_timedlock(&mutex, &until_time)");
            return lock_success;
        }
        return lock_failed; 
    }
    virtual lock_status try_lock_detached(mutex_t& mutex) const { 
        int err = 0;
        LOGGER_IS_LOGGED_TRACE("::mutex_trylock(&mutex)...");
        if ((err = ::mutex_trylock(&mutex))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::mutex_trylock(&mutex)");
                return lock_busy;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_trylock(&mutex)");
                return lock_failed;
            }
        } else {
            LOGGER_IS_LOGGED_TRACE("...::mutex_trylock(&mutex)");
            return lock_success;
        }
        return lock_failed; 
    }
    virtual lock_status untimed_lock_detached(mutex_t& mutex) const { 
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::mutex_lock(&mutex)...");
        if ((err = ::mutex_lock(&mutex))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_ERROR("...EBUSY err = "<< err << " on ::mutex_lock(&mutex)");
                return lock_busy;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_lock(&mutex)");
                return lock_failed;
            }
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::mutex_lock(&mutex)");
            return lock_success;
        }
        return lock_failed; 
    }
    virtual bool unlock_detached(mutex_t& mutex) const { 
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::mutex_unlock(&mutex)...");
        if (!(err = ::mutex_unlock(&mutex))) {
            LOGGER_IS_LOGGED_DEBUG("...::mutex_unlock(&mutex)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_unlock(&mutex)");
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(mutex_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached(mutex_t& mutex) const {
        attached_t detached = (attached_t)(unattached);
        int err = 0, type = USYNC_PROCESS | LOCK_ROBUST;
        void *arg = NULL;
        LOGGER_IS_LOGGED_DEBUG("::mutex_init(&mutex, type, arg)...");
        if (!(err = ::mutex_init(&mutex, type, arg))) {
            LOGGER_IS_LOGGED_DEBUG("...::mutex_init(&mutex, type, arg)");
            detached = &mutex;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_init(&mutex, type, arg)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            int err = 0;
            LOGGER_IS_LOGGED_DEBUG("::mutex_destroy(detached)...");
            if (((err = ::mutex_destroy(detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::mutex_destroy(detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::mutex_destroy(detached)");
                return true;
            }
        }
        return false;
    }

protected:
    mutex_t mutex_;
}; /// class mutext
typedef mutext<> mutex;

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_ORACLE_SOLARIS_MUTEX_HPP 
