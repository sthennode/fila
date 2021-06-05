///////////////////////////////////////////////////////////////////////
// Copyright (c) 1988-2018 $organization$
//
// This software is provided by the author and contributors ``as is'' 
// and any express or implied warranties, including, but not limited to, 
// the implied warranties of merchantability and fitness for a particular 
// purpose are disclaimed. In no event shall the author or contributors 
// be liable for any direct, indirect, incidental, special, exemplary, 
// or consequential damages (including, but not limited to, procurement 
// of substitute goods or services; loss of use, data, or profits; or 
// business interruption) however caused and on any theory of liability, 
// whether in contract, strict liability, or tort (including negligence 
// or otherwise) arising in any way out of the use of this software, 
// even if advised of the possibility of such damage.
//
//   File: semaphore.hpp
//
// Author: $author$
//   Date: 8/16/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_APPLE_MACH_SEMAPHORE_HPP
#define _XOS_MT_APPLE_MACH_SEMAPHORE_HPP

#include <mach/task.h>
#include <mach/mach.h>
#include <mach/semaphore.h>

#include "xos/mt/semaphore/extend.hpp"
#include "xos/logger/interface.hpp"

namespace xos {
namespace mt {
namespace apple {
namespace mach {

//typedef uint64_t semaphore_t;
typedef semaphore_t* semaphore_attached_t;

///////////////////////////////////////////////////////////////////////
//  Class: semaphoret
///////////////////////////////////////////////////////////////////////
template <class Textends = mt::semaphore::extendt<semaphore_attached_t> >

class _EXPORT_CLASS semaphoret: public Textends {
public:
    typedef Textends extends;

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

    virtual bool acquire() {
        if (acquire_success == (this->untimed_acquire())) {
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
        return this->timed_acquire(0);
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

    virtual semaphore_t* create_attached() {
        return create_attached(0);
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
            IS_ERR_LOGGED_DEBUG("...::semaphore_create(task, &semaphore, sync_policy, initially_released)");
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
                IS_ERR_LOGGED_DEBUG("...::semaphore_destroy(task, *semaphore)")
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_destroy(task, *semaphore)");
            }
        }
        return false;
    }

protected:
    semaphore_t m_semaphore;
}; // class _EXPORT_CLASS semaphoret

typedef semaphoret<> semaphore;

} // namespace mach
} // namespace apple
} // namespace mt
} // namespace xos

#endif // _XOS_MT_APPLE_MACH_SEMAPHORE_HPP 
