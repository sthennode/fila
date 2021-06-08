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
///   File: mutex.cpp
///
/// Author: $author$
///   Date: 1/1/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/platform/mutex.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace platform {
namespace os {
namespace platform {

typedef mt::os::mutex mutex;

} /// namespace platform
} /// namespace os
} /// namespace platform
} /// namespace xos

///
/// platform mutexes
/// ...
platform_mutex_error_t platform_mutex_create(platform_mutex_t* mutex, platform_mutex_attr_t attr) { 
    if ((mutex)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = new ::xos::platform::os::platform::mutex(((::xos::platform::os::platform::mutex::attached_t)::xos::platform::os::platform::mutex::unattached), false, false))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            if ((mtx->create())) {
                *mutex = mtx;
                err = platform_mutex_error_success;
            } else {
                delete mtx;
            }
            return err;
        }
    }
    return platform_mutex_error_failed; 
}
platform_mutex_error_t platform_mutex_destroy(platform_mutex_t* mutex) { 
    if ((mutex)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::platform::mutex*)(*mutex)))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            if ((mtx->destroy())) {
                err = platform_mutex_error_success;
            }
            *mutex = 0;
            delete mtx;
            return err;
        }
    }
    return platform_mutex_error_failed; 
                                                                       }
platform_mutex_error_t platform_mutex_lock(platform_mutex_t* mutex) { 
    if ((mutex)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::platform::mutex*)(*mutex)))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            if ((mtx->lock())) {
                err = platform_mutex_error_success;
            }
            return err;
        }
    }
    return platform_mutex_error_failed; 
}
platform_mutex_error_t platform_mutex_timedlock(platform_mutex_t* mutex, platform_mutex_timeout_t* timeout) { 
    if ((mutex) && (timeout)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::platform::mutex*)(*mutex)))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            mseconds_t milliseconds = ::xos::seconds_mseconds(timeout->tv_sec) + ::xos::nseconds_mseconds(timeout->tv_nsec);
            ::xos::lock_status status = ::xos::lock_failed;
            if (::xos::lock_success == (mtx->timed_lock(milliseconds))) {
                err = platform_mutex_error_success;
            }
            return err;
        }
    }
    return platform_mutex_error_failed; 
}
platform_mutex_error_t platform_mutex_trylock(platform_mutex_t* mutex) { 
    if ((mutex)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::platform::mutex*)(*mutex)))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            ::xos::lock_status status = ::xos::lock_failed;
            if (::xos::lock_success == (mtx->try_lock())) {
                err = platform_mutex_error_success;
            }
            return err;
        }
    }
    return platform_mutex_error_failed; 
}
platform_mutex_error_t platform_mutex_unlock(platform_mutex_t* mutex) { 
    if ((mutex)) {
        ::xos::platform::os::platform::mutex* mtx = 0;
        if ((mtx = ((::xos::platform::os::platform::mutex*)(*mutex)))) {
            platform_mutex_error_t err = platform_mutex_error_failed;
            if ((mtx->unlock())) {
                err = platform_mutex_error_success;
            }
            return err;
        }
    }
    return platform_mutex_error_failed; 
}
/// ...
/// platform mutexes
/// 
