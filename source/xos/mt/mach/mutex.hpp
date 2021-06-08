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
///   Date: 12/31/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MACH_MUTEX_HPP
#define XOS_MT_MACH_MUTEX_HPP

#include "xos/mt/mutex.hpp"
#include "xos/base/time.hpp"

#if !defined(MACH) || defined(APPLE)
#include "xos/platform/os/mach/lock_set.h"
#endif /// !defined(MACH) || defined(APPLE)

///
/// kern_return_t lock_timed
/// 
#if !defined(HAS_LOCK_TIMED)
inline kern_return_t lock_timed
(lock_set_t lock_set, int lock_id, mach_timespec_t timeout) {
    kern_return_t err = KERN_SUCCESS;
    for (useconds_t time_useconds = 0,
         timeout_useconds = ::xos::seconds_useconds(timeout.tv_sec) + ::xos::nseconds_useconds(timeout.tv_nsec),
         useconds_sleep = ::xos::mseconds_useconds(10);
         timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
        if (KERN_SUCCESS == (err = ::lock_try(lock_set, lock_id))) {
            return err;
        }
        ::usleep(useconds_sleep);
    }
    return KERN_OPERATION_TIMED_OUT;
}
#define HAS_LOCK_TIMED
#endif /// !defined(HAS_LOCK_TIMED)

namespace xos {
namespace mt {
namespace mach {

typedef lock_set_t mutex_t;
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

    /// ...lock... / ...unlock...
    virtual bool lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return lock_detached(*detached, 0);
        }
        return false;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_lock_detached(*detached, 0, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_lock_detached(*detached, 0);
        }
        return lock_failed;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_lock_detached(*detached, 0);
        }
        return lock_failed;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return unlock_detached(*detached, 0);
        }
        return false;
    }

    /// ...lock... / ...unlock...
    virtual bool lock_detached(mutex_t& mutex, int lock_id) const { 
        return (lock_success == (untimed_lock_detached(mutex, lock_id))); 
    }
    virtual lock_status timed_lock_detached(mutex_t& mutex, int lock_id, mseconds_t milliseconds) const { 
        kern_return_t err = KERN_SUCCESS;
        mach_timespec_t wait_time;
        wait_time.tv_sec = mseconds_seconds(milliseconds);
        wait_time.tv_nsec = mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IS_LOGGED_TRACE("::lock_timed(mutex, lock_id, wait_time)...");
        if (KERN_SUCCESS == (err = ::lock_timed(mutex, lock_id, wait_time))) {
            LOGGER_IS_LOGGED_TRACE("...::lock_timed(mutex, lock_id, wait_time)");
            return lock_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                LOGGER_IS_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                return lock_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                    return lock_interrupted;
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::lock_timed(mutex, lock_id, wait_time)");
                }
            }
        }
        return lock_failed; 
    }
    virtual lock_status untimed_lock_detached(mutex_t& mutex, int lock_id) const { 
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::lock_acquire(mutex, lock_id = " << lock_id << ")...");
        if (KERN_SUCCESS == (err = ::lock_acquire(mutex, lock_id))) {
            LOGGER_IS_LOGGED_DEBUG("...::lock_acquire(mutex, lock_id = " << lock_id << ")");
            return lock_success;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::lock_acquire(mutex, lock_id = " << lock_id << ")");
        }
        return lock_failed; 
    }
    virtual lock_status try_lock_detached(mutex_t& mutex, int lock_id) const { 
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_TRACE("::lock_try(mutex, lock_id = " << lock_id << ")...");
        if (KERN_SUCCESS == (err = ::lock_try(mutex, lock_id))) {
            LOGGER_IS_LOGGED_TRACE("...::lock_try(mutex, lock_id = " << lock_id << ")");
            return lock_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                LOGGER_IS_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::lock_try(mutex, lock_id = " << lock_id << ")");
                return lock_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::lock_try(mutex, lock_id = " << lock_id << ")");
                    return lock_interrupted;
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::lock_try(mutex, lock_id = " << lock_id << ")");
                }
            }
        }
        return lock_failed; 
    }
    virtual bool unlock_detached(mutex_t& mutex, int lock_id) const { 
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::lock_release(mutex, lock_id = " << lock_id << ")...");
        if (KERN_SUCCESS == (err = ::lock_release(mutex, lock_id))) {
            LOGGER_IS_LOGGED_DEBUG("...::lock_release(mutex, lock_id = " << lock_id << ")");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::lock_release(mutex, lock_id = " << lock_id << ")");
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
        task_t task = ::mach_task_self();
        sync_policy_t policy = SYNC_POLICY_FIFO;
        kern_return_t err = KERN_SUCCESS;
        
        LOGGER_IS_LOGGED_DEBUG("::lock_set_create(task, &mutex, 1, policy)...");
        if (KERN_SUCCESS != (err = ::lock_set_create(task, &mutex, 1, policy))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::lock_set_create(task, &mutex, 1, policy)...");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::lock_set_create(task, &mutex, 1, policy)");
            detached = &mutex;
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            task_t task = ::mach_task_self();
            kern_return_t err = KERN_SUCCESS;
            LOGGER_IS_LOGGED_DEBUG("::lock_set_destroy(task, *detached)...");
            if (KERN_SUCCESS != ((err = ::lock_set_destroy(task, *detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::lock_set_destroy(task, *detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::lock_set_destroy(task, *detached)");
                return true;
            }
        }
        return false;
    }

protected:
    mutex_t mutex_;
}; /// class mutext
typedef mutext<> mutex;

} /// namespace mach
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_MACH_MUTEX_HPP 
