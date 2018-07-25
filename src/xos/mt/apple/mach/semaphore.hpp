///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2018 $organization$
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
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_APPLE_MACH_SEMAPHORE_HPP
#define _XOS_MT_APPLE_MACH_SEMAPHORE_HPP

// /source/experimental/fila/cpp/nadir/src/xos/mt/apple/mach/semaphore.hpp

#include <mach/task.h>
#include <mach/mach.h>
#include <mach/semaphore.h>

#include "xos/mt/semaphore.hpp"

namespace xos {
namespace mt {
namespace apple {
namespace mach {

typedef semaphore_t* semaphore_attached_t;
typedef mt::semaphore semaphoret_implements;
typedef mt::extended::semaphoret<semaphore_attached_t> semaphoret_extends;
///////////////////////////////////////////////////////////////////////
///  Class: semaphoret
///////////////////////////////////////////////////////////////////////
template <class TImplements = semaphoret_implements, class TExtends = semaphoret_extends>
class _EXPORT_CLASS semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    semaphoret(bool is_logged): extends(is_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    semaphoret(const semaphoret &copy): extends(copy) {
    }
    semaphoret() {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~semaphoret() {
        IS_ERR_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    virtual bool create(size_t initially_released) {
        semaphore_t* detached = 0;

        if ((detached = create_attached(initially_released))) {
            this->set_is_created();
            return detached;
        }
        return false;
    }
    virtual bool create() {
        return create(0);
    }
    virtual bool destroy() {
        semaphore_t* detached = 0;

        if ((detached = this->detach())) {
            if ((destroy_detached(detached))) {
                return true;
            }
        }
        return false;
    }

    virtual semaphore_t* create_attached(size_t initially_released) {
        semaphore_t* detached = 0;

        if ((this->destroyed())) {
            if ((detached = create_detached(m_semaphore, initially_released))) {
                this->attach(detached);
                return detached;
            }
        }
        return 0;
    }
    virtual semaphore_t* create_detached
    (semaphore_t& semaphore, size_t initially_released) const {
        int err = 0;
        task_t task = mach_task_self();
        sync_policy_t sync_policy = SYNC_POLICY_FIFO;

        IS_ERR_LOGGED_DEBUG("::semaphore_create(task, &semaphore, sync_policy, initially_released)...");
        if (KERN_SUCCESS == (err = ::semaphore_create(task, &semaphore, sync_policy, initially_released))) {
            return &semaphore;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_create(task, &semaphore, sync_policy, initially_released)");
        }
        return 0;
    }
    virtual bool destroy_detached(semaphore_t* semaphore) const {
        if ((semaphore)) {
            int err = 0;
            task_t task = mach_task_self();

            IS_ERR_LOGGED_DEBUG("::semaphore_destroy(task, *semaphore)...")
            if (KERN_SUCCESS == (err = ::semaphore_destroy(task, *semaphore))) {
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_destroy(task, *semaphore)");
            }
        }
        return false;
    }

    virtual bool acquire() {
        if (acquire_success == (untimed_acquire())) {
            return true;
        }
        return false;
    }
    virtual bool release() {
        semaphore_t* semaphore = 0;

        if ((semaphore = this->attached_to())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_signal(*semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_signal(*semaphore))) {
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(*semaphore)");
            }
        }
        return false;
    }
    virtual acquire_status try_acquire() {
        return timed_acquire(0);
    }
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        if (0 > (milliseconds)) {
            return untimed_acquire();
        } else {
            semaphore_t* semaphore = 0;

            if ((semaphore = this->attached_to())) {
                int err = 0;
                mach_timespec_t wait_time;
                wait_time.tv_sec = mseconds_seconds(milliseconds);
                wait_time.tv_nsec = mseconds_nseconds(milliseconds);

                IS_ERR_LOGGED_TRACE("::semaphore_timedwait(*semaphore, wait_time)...");
                if (KERN_SUCCESS == (err = ::semaphore_timedwait(*semaphore, wait_time))) {
                    return acquire_success;
                } else {
                    if (KERN_OPERATION_TIMED_OUT == (err)) {
                        IS_ERR_LOGGED_TRACE("...failed err = KERN_OPERATION_TIMED_OUT on ::semaphore_timedwait(*semaphore, wait_time)");
                        return acquire_busy;
                    } else {
                        if (KERN_ABORTED == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed err = KERN_ABORTED on ::semaphore_timedwait(*semaphore, wait_time)");
                            return acquire_interrupted;
                        } else {
                            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_timedwait(*semaphore, wait_time)");
                        }
                    }
                }
            }
        }
        return acquire_failed;
    }
    virtual acquire_status untimed_acquire() {
        semaphore_t* semaphore = 0;

        if ((semaphore = this->attached_to())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_wait(*semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_wait(*semaphore))) {
                return acquire_success;
            } else {
                if (KERN_OPERATION_TIMED_OUT == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed err = KERN_OPERATION_TIMED_OUT on ::semaphore_wait(*semaphore)");
                    return acquire_busy;
                } else {
                    if (KERN_ABORTED == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed err = KERN_ABORTED on ::semaphore_wait(*semaphore)");
                        return acquire_interrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_wait(*semaphore)");
                    }
                }
            }
        }
        return acquire_failed;
    }

protected:
    semaphore_t m_semaphore;
};
typedef semaphoret<> semaphore;

} /// namespace mach
} /// namespace apple
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_APPLE_MACH_SEMAPHORE_HPP 
