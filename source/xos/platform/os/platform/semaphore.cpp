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
///   Date: 1/5/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/platform/semaphore.hpp"
#include "xos/mt/os/semaphore.hpp"

namespace xos {
namespace platform {
namespace os {
namespace platform {

typedef mt::os::semaphore semaphore;

} /// namespace platform
} /// namespace os
} /// namespace platform
} /// namespace xos

///
/// platform semaphores
/// ...
platform_semaphore_error_t platform_semaphore_create(platform_semaphore_t* semaphore, platform_semaphore_attr_t attr) {
    if ((semaphore)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = new ::xos::platform::os::platform::semaphore(((::xos::platform::os::platform::semaphore::attached_t)::xos::platform::os::platform::semaphore::unattached), false, false))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            if ((sem->create())) {
                *semaphore = sem;
                err = platform_semaphore_error_success;
            } else {
                delete sem;
            }
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
platform_semaphore_error_t platform_semaphore_destroy(platform_semaphore_t* semaphore) {
    if ((semaphore)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = ((::xos::platform::os::platform::semaphore*)(*semaphore)))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            if ((sem->destroy())) {
                err = platform_semaphore_error_success;
            }
            *semaphore = 0;
            delete sem;
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
platform_semaphore_error_t platform_semaphore_acquire(platform_semaphore_t* semaphore) {
    if ((semaphore)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = ((::xos::platform::os::platform::semaphore*)(*semaphore)))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            if ((sem->acquire())) {
                err = platform_semaphore_error_success;
            }
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
platform_semaphore_error_t platform_semaphore_timed_acquire(platform_semaphore_t* semaphore, platform_semaphore_timeout_t* timeout) {
    if ((semaphore) && (timeout)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = ((::xos::platform::os::platform::semaphore*)(*semaphore)))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            ::xos::acquire_status status = ::xos::acquire_failed;
            mseconds_t milliseconds = ::xos::seconds_mseconds(timeout->tv_sec) + ::xos::nseconds_mseconds(timeout->tv_nsec);
            if (::xos::acquire_success == (status = sem->timed_acquire(milliseconds))) {
                err = platform_semaphore_error_success;
            } else {
                if (::xos::acquire_busy == (status)) {
                    err = platform_semaphore_error_busy;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        err = platform_semaphore_error_timeout;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            err = platform_semaphore_error_interrupted;
                        } else {
                        }
                    }
                }
            }
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
platform_semaphore_error_t platform_semaphore_try_acquire(platform_semaphore_t* semaphore) {
    if ((semaphore)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = ((::xos::platform::os::platform::semaphore*)(*semaphore)))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            ::xos::acquire_status status = ::xos::acquire_failed;
            if (::xos::acquire_success == (status = sem->try_acquire())) {
                err = platform_semaphore_error_success;
            } else {
                if (::xos::acquire_busy == (status)) {
                    err = platform_semaphore_error_busy;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        err = platform_semaphore_error_timeout;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            err = platform_semaphore_error_interrupted;
                        } else {
                        }
                    }
                }
            }
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
platform_semaphore_error_t platform_semaphore_release(platform_semaphore_t* semaphore) {
    if ((semaphore)) {
        ::xos::platform::os::platform::semaphore* sem = 0;
        if ((sem = ((::xos::platform::os::platform::semaphore*)(*semaphore)))) {
            platform_semaphore_error_t err = platform_semaphore_error_failed;
            if ((sem->release())) {
                err = platform_semaphore_error_success;
            }
            return err;
        }
    }
    return platform_semaphore_error_failed;
}
/// ...
/// platform semaphores
/// 
