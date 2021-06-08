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
///   File: lock_set.cpp
///
/// Author: $author$
///   Date: 12/31/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/mach/lock_set.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace platform {
namespace os {
namespace mach {

typedef mt::os::mutex mutex;

} /// namespace mach
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(MACH) || defined(APPLE)
///
/// mach lock_set
/// ...
kern_return_t lock_set_create (
        task_t task,
        lock_set_t *new_lock_set,
        int n_ulocks,
        int policy
) {
    if ((0 != new_lock_set) && (1 == n_ulocks) && (SYNC_POLICY_FIFO == policy)) {
        try {
            ::xos::platform::os::mach::mutex* mtx = 0;
            if ((mtx = new ::xos::platform::os::mach::mutex(((::xos::platform::os::mach::mutex::attached_t)::xos::platform::os::mach::mutex::unattached), false, false))) {
                kern_return_t err = KERN_FAILURE;
                if ((mtx->create())) {
                    *new_lock_set = mtx;
                    err = KERN_SUCCESS;
                } else {
                    delete mtx;
                }
                return err;
            }
        } catch (...) {
            return KERN_MEMORY_ERROR;
        }
    }
    return KERN_FAILURE;
}

kern_return_t lock_set_destroy (
        task_t task,
        lock_set_t lock_set
) {
    if ((lock_set)) {
        ::xos::platform::os::mach::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::mach::mutex*)(lock_set)))) {
            kern_return_t err = KERN_FAILURE;
            if ((mtx->destroy())) {
                err = KERN_SUCCESS;
            }
            try {
                delete mtx;
            } catch (...) {
                err = KERN_MEMORY_ERROR;
            }
            return err;
        }
    }
    return KERN_FAILURE;
}

kern_return_t lock_acquire (
        lock_set_t lock_set,
        int lock_id
) {
    if ((lock_set) && (!lock_id)) {
        ::xos::platform::os::mach::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::mach::mutex*)(lock_set)))) {
            kern_return_t err = KERN_FAILURE;
            if ((mtx->lock())) {
                err = KERN_SUCCESS;
            }
            return err;
        }
    }
    return KERN_FAILURE;
}

kern_return_t lock_release (
        lock_set_t lock_set,
        int lock_id
) {
    if ((lock_set) && (!lock_id)) {
        ::xos::platform::os::mach::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::mach::mutex*)(lock_set)))) {
            kern_return_t err = KERN_FAILURE;
            if ((mtx->unlock())) {
                err = KERN_SUCCESS;
            }
            return err;
        }
    }
    return KERN_FAILURE;
}

kern_return_t lock_try (
        lock_set_t lock_set,
        int lock_id
) {
    if ((lock_set) && (!lock_id)) {
        ::xos::platform::os::mach::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::mach::mutex*)(lock_set)))) {
            kern_return_t err = KERN_FAILURE;
            ::xos::lock_status status = ::xos::lock_failed;
            if (::xos::lock_success == (status = mtx->try_lock())) {
                err = KERN_SUCCESS;
            } else {
                if (::xos::lock_busy == (status)) {
                    err = KERN_OPERATION_TIMED_OUT;
                } else {
                    if (::xos::lock_interrupted == (status)) {
                        err = KERN_ABORTED;
                    } else {
                    }
                }
            }
            return err;
        }
    }
    return KERN_FAILURE;
}
/// ...
/// mach lock_set
///
#endif /// !defined(MACH) || defined(APPLE)
