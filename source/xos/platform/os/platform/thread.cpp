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
///   File: thread.cpp
///
/// Author: $author$
///   Date: 1/20/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/platform/thread.hpp"
#include "xos/mt/os/thread.hpp"
#include "xos/mt/thread.hpp"

namespace xos {
namespace platform {
namespace os {
namespace platform {

/// class threadt
template <class TExtends = mt::os::thread, class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;

    typedef typename extends::ran_t ran_t;
    
    /// constructor / destructor
    threadt(ran_t &ran, platform_thread_start_routine_t start_routine): extends(0, false, false), ran_(ran), start_routine_(start_routine) {
        if (!(this->create())) {
            throw (create_exception(create_failed));
        }
    }
    threadt(const threadt& copy): extends(copy), ran_(null_ran_), start_routine_(copy.start_routine_) {
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            throw join_exception(join_failed);
        }
    }
    
protected:
    /// run
    virtual pointer_t run(pointer_t parameter = 0) {
        pointer_t result = 0;
        if ((start_routine_) && (parameter = ((pointer_t)(&ran_)))) {
            result = start_routine_(parameter);
        }
        return result;
    }

protected:
    ran_t null_ran_, &ran_;
    platform_thread_start_routine_t start_routine_;
}; /// class threadt
typedef threadt<> thread;

} /// namespace platform
} /// namespace os
} /// namespace platform
} /// namespace xos

///
/// platform threads
/// ...
platform_thread_error_t platform_thread_create(platform_thread_t* thread, platform_thread_attr_t attr, platform_thread_start_routine_t start_routine) {
    ::xos::platform::os::platform::thread::ran_t* ran = 0;
    if ((thread) && (start_routine) && ((ran = ((::xos::platform::os::platform::thread::ran_t*)attr)))) {
        ::xos::platform::os::platform::thread* trd = 0;
        try {
            if ((trd = new ::xos::platform::os::platform::thread(*ran, start_routine))) {
                *thread = ((platform_thread_t)trd);
                return platform_thread_error_success;
            }
        } catch (...) {
        }
    }
    return platform_thread_error_failed;
}
platform_thread_error_t platform_thread_destroy(platform_thread_t* thread) {
    if ((thread)) {
        ::xos::platform::os::platform::thread* trd = 0;
        if ((trd = ((::xos::platform::os::platform::thread*)(*thread)))) {
            platform_thread_error_t err = platform_thread_error_failed;
            if ((trd->destroy())) {
                err = platform_thread_error_success;
            }
            try {
                delete trd;
            } catch (...) {
                err = platform_thread_error_failed;
            }
            *thread = 0;
            return err;
        }
    }
    return platform_thread_error_failed;
}
platform_thread_error_t platform_thread_join(platform_thread_t* thread) {
    if ((thread)) {
        ::xos::platform::os::platform::thread* trd = 0;
        if ((trd = ((::xos::platform::os::platform::thread*)(*thread)))) {
            ::xos::join_status status = ::xos::join_failed;
            if (::xos::join_success == (status = trd->untimed_join())) {
                trd->set_is_joined();
                return platform_thread_error_success;
            } else {
                switch (status) {
                case ::xos::join_busy:
                    return platform_thread_error_busy;
                case ::xos::join_timeout:
                    return platform_thread_error_timeout;
                case ::xos::join_interrupted:
                    trd->set_is_joined();
                    return platform_thread_error_interrupted;
                }
                trd->set_is_joined();
            }
        }
    }
    return platform_thread_error_failed;
}
platform_thread_error_t platform_thread_time_join(platform_thread_t* thread, platform_thread_timeout_t* timeout) {
    if ((thread)) {
        ::xos::platform::os::platform::thread* trd = 0;
        if ((trd = ((::xos::platform::os::platform::thread*)(*thread)))) {
            mseconds_t mseconds = ::xos::seconds_mseconds(timeout->tv_sec) + ::xos::nseconds_mseconds(timeout->tv_nsec);
            ::xos::join_status status = ::xos::join_failed;
            if (::xos::join_success == (status = trd->time_join(mseconds))) {
                trd->set_is_joined();
                return platform_thread_error_success;
            } else {
                switch (status) {
                case ::xos::join_busy:
                    return platform_thread_error_busy;
                case ::xos::join_timeout:
                    return platform_thread_error_timeout;
                case ::xos::join_interrupted:
                    trd->set_is_joined();
                    return platform_thread_error_interrupted;
                }
                trd->set_is_joined();
            }
        }
    }
    return platform_thread_error_failed;
}
platform_thread_error_t platform_thread_timed_join(platform_thread_t* thread, platform_thread_timeout_t* timeout) {
    if ((thread)) {
        ::xos::platform::os::platform::thread* trd = 0;
        if ((trd = ((::xos::platform::os::platform::thread*)(*thread)))) {
            mseconds_t mseconds = ::xos::seconds_mseconds(timeout->tv_sec) + ::xos::nseconds_mseconds(timeout->tv_nsec);
            ::xos::join_status status = ::xos::join_failed;
            if (::xos::join_success == (status = trd->timed_join(mseconds))) {
                trd->set_is_joined();
                return platform_thread_error_success;
            } else {
                switch (status) {
                case ::xos::join_busy:
                    return platform_thread_error_busy;
                case ::xos::join_timeout:
                    return platform_thread_error_timeout;
                case ::xos::join_interrupted:
                    trd->set_is_joined();
                    return platform_thread_error_interrupted;
                }
                trd->set_is_joined();
            }
        }
    }
    return platform_thread_error_failed;
}
platform_thread_error_t platform_thread_try_join(platform_thread_t* thread) {
    if ((thread)) {
        ::xos::platform::os::platform::thread* trd = 0;
        if ((trd = ((::xos::platform::os::platform::thread*)(*thread)))) {
            ::xos::join_status status = ::xos::join_failed;
            if (::xos::join_success == (status = trd->try_join())) {
                trd->set_is_joined();
                return platform_thread_error_success;
            } else {
                switch (status) {
                case ::xos::join_busy:
                    return platform_thread_error_busy;
                case ::xos::join_timeout:
                    return platform_thread_error_timeout;
                case ::xos::join_interrupted:
                    trd->set_is_joined();
                    return platform_thread_error_interrupted;
                }
                trd->set_is_joined();
            }
        }
    }
    return platform_thread_error_failed;
}
/// ...
/// platform threads
///