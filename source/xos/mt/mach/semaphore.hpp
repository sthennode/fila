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
///   Date: 1/11/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MACH_SEMAPHORE_HPP
#define XOS_MT_MACH_SEMAPHORE_HPP

#include "xos/mt/semaphore.hpp"

#if defined(MACH)
#include <mach/task.h>
#include <mach/mach.h>
#include <mach/semaphore.h>
#else /// defined(MACH)
#include "xos/platform/os/mach/semaphore.h"
#endif /// defined(MACH)

namespace xos {
namespace mt {
namespace mach {

/// class semaphoret
template 
<typename TWhat = semaphore_t,
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

    /// destroy
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return destroy_detached(*detached);
        }
        return false;
    }

    /// ...acquire... / ...release...
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
    virtual bool acquire_detached(semaphore_t& semaphore) const { 
        return (acquire_success == (untimed_acquire_detached(semaphore))); 
    }
    virtual acquire_status timed_acquire_detached(semaphore_t& semaphore, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            kern_return_t err = KERN_SUCCESS;
            mach_timespec_t wait_time;
            wait_time.tv_sec = mseconds_seconds(milliseconds);
            wait_time.tv_nsec = mseconds_nseconds(mseconds_mseconds(milliseconds));
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::semaphore_timedwait(semaphore, wait_time)...");
            if (KERN_SUCCESS == ((err = ::semaphore_timedwait(semaphore, wait_time)))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::semaphore_timedwait(semaphore, wait_time)");
                return acquire_success;
            } else {
                if (KERN_OPERATION_TIMED_OUT == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...failed KERN_OPERATION_TIMED_OUT err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                    return acquire_busy;
                } else {
                    if (KERN_ABORTED == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...failed KERN_ABORTED err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                        return acquire_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(semaphore);
            } else {
                return try_acquire_detached(semaphore);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(semaphore_t& semaphore) const { 
        mseconds_t milliseconds = 0;
        kern_return_t err = KERN_SUCCESS;
        mach_timespec_t wait_time;
        wait_time.tv_sec = mseconds_seconds(milliseconds);
        wait_time.tv_nsec = mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IS_LOGGED_TRACE("::semaphore_timedwait(semaphore, wait_time)...");
        if (KERN_SUCCESS == ((err = ::semaphore_timedwait(semaphore, wait_time)))) {
            LOGGER_IS_LOGGED_TRACE("...::semaphore_timedwait(semaphore, wait_time)");
            return acquire_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                LOGGER_IS_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                return acquire_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...failed KERN_ABORTED err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                    return acquire_interrupted;
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::semaphore_timedwait(semaphore, wait_time)");
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(semaphore_t& semaphore) const { 
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::semaphore_wait(semaphore)...");
        if (KERN_SUCCESS == ((err = ::semaphore_wait(semaphore)))) {
            LOGGER_IS_LOGGED_DEBUG("...::semaphore_wait(semaphore)");
            return acquire_success;
        } else {
            if (KERN_OPERATION_TIMED_OUT == (err)) {
                LOGGER_IS_LOGGED_ERROR("...failed KERN_OPERATION_TIMED_OUT err = "<< err << " ::semaphore_wait(semaphore)");
                return acquire_busy;
            } else {
                if (KERN_ABORTED == (err)) {
                    LOGGER_IS_LOGGED_ERROR("...failed KERN_ABORTED err = "<< err << " ::semaphore_wait(semaphore)");
                    return acquire_interrupted;
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::semaphore_wait(semaphore)");
                }
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(semaphore_t& semaphore) const { 
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::semaphore_signal(semaphore)...");
        if (KERN_SUCCESS != ((err = ::semaphore_signal(semaphore)))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::semaphore_signal(semaphore)");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::semaphore_signal(semaphore)");
            return true;
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
    virtual attached_t create_detached(semaphore_t& semaphore) const {
        return create_detached(semaphore, 0);
    }
    virtual attached_t create_detached(semaphore_t& semaphore, size_t released) const {
        attached_t detached = (attached_t)(unattached);
        task_t task = ::mach_task_self();
        sync_policy_t policy = SYNC_POLICY_FIFO;
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::semaphore_create(task, &semaphore, policy, released)...");
        if (KERN_SUCCESS != (err = ::semaphore_create(task, &semaphore, policy, released))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_create(task, &semaphore, policy, released)...");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::semaphore_create(task, &semaphore, policy, released)");
            detached = &semaphore;
        }
        return detached;
    }
    virtual bool destroy_detached(semaphore_t& semaphore) const {
        task_t task = ::mach_task_self();
        kern_return_t err = KERN_SUCCESS;
        LOGGER_IS_LOGGED_DEBUG("::semaphore_destroy(task, semaphore)...");
        if (KERN_SUCCESS != ((err = ::semaphore_destroy(task, semaphore)))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::semaphore_destroy(task, semaphore)");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::semaphore_destroy(task, semaphore)");
            return true;
        }
        return false;
    }
    
protected:
    semaphore_t semaphore_;
}; /// class semaphoret
typedef semaphoret<> semaphore;

} /// namespace mach
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_MACH_SEMAPHORE_HPP 
