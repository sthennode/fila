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
///   File: semaphore.cpp
///
/// Author: $author$
///   Date: 1/12/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/mach/semaphore.hpp"
#include "xos/mt/os/semaphore.hpp"

namespace xos {
namespace platform {
namespace os {
namespace mach {

typedef mt::os::semaphore semaphore;

} /// namespace mach
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(MACH)
///
/// mach semaphores
/// ...
///
kern_return_t semaphore_create(task_t task, semaphore_t *semaphore, int policy, int value) {
    if ((semaphore) && (value >= 0)) {
        kern_return_t err = KERN_FAILURE;
        ::xos::platform::os::mach::semaphore* sem = 0;
        try {
            if ((sem = new ::xos::platform::os::mach::semaphore(((::xos::platform::os::mach::semaphore::attached_t)::xos::platform::os::mach::semaphore::unattached), false, false))) {
                if ((sem->create(value))) {
                    *semaphore = ((semaphore_t)sem);
                    return KERN_SUCCESS;
                }
                delete sem;
            }
        } catch (...) {
            err = KERN_MEMORY_ERROR;
        }
        return err;
    }
    return KERN_FAILURE;
}
kern_return_t semaphore_destroy(task_t task, semaphore_t semaphore) {
    ::xos::platform::os::mach::semaphore* sem = 0;
    if ((sem = ((::xos::platform::os::mach::semaphore*)semaphore))) {
        kern_return_t err = KERN_FAILURE;
        if ((sem->destroy())) {
            err = KERN_SUCCESS;
        }
        try {
            delete sem;
        } catch (...) {
            err = KERN_MEMORY_ERROR;
        }
        return err;
    }
    return KERN_FAILURE;
}

kern_return_t semaphore_signal(semaphore_t semaphore) {
    ::xos::platform::os::mach::semaphore* sem = 0;
    if ((sem = ((::xos::platform::os::mach::semaphore*)semaphore))) {
        if ((sem->release())) {
            return KERN_SUCCESS;
        }
    }
    return KERN_FAILURE;
}
kern_return_t semaphore_wait(semaphore_t semaphore) {
    ::xos::platform::os::mach::semaphore* sem = 0;
    if ((sem = ((::xos::platform::os::mach::semaphore*)semaphore))) {
        ::xos::acquire_status status = ::xos::acquire_failed;
        if (::xos::acquire_success == (status = sem->untimed_acquire())) {
            return KERN_SUCCESS;
        } else {
            switch (status) {
            case ::xos::acquire_busy:
            case ::xos::acquire_timeout:
                return KERN_OPERATION_TIMED_OUT;

            case ::xos::acquire_interrupted:
                return KERN_ABORTED;
            }
        }
    }
    return KERN_FAILURE;
}
kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time) {
    ::xos::platform::os::mach::semaphore* sem = 0;
    if ((sem = ((::xos::platform::os::mach::semaphore*)semaphore))) {
        mseconds_t milliseconds = ::xos::seconds_mseconds(wait_time.tv_sec) + ::xos::nseconds_mseconds(wait_time.tv_nsec);
        ::xos::acquire_status status = ::xos::acquire_failed;
        if (::xos::acquire_success == (status = sem->timed_acquire(milliseconds))) {
            return KERN_SUCCESS;
        } else {
            switch (status) {
            case ::xos::acquire_busy:
            case ::xos::acquire_timeout:
                return KERN_OPERATION_TIMED_OUT;

            case ::xos::acquire_interrupted:
                return KERN_ABORTED;
            }
        }
    }
    return KERN_FAILURE;
}
///
/// ...
/// mach semaphores
///
#endif /// !defined(MACH) ///


