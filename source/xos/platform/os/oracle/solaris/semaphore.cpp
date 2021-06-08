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
///   Date: 1/9/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/oracle/solaris/semaphore.hpp"
#include "xos/mt/os/semaphore.hpp"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

///
/// clock_gettime
///
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#if !defined(CLOCK_HAS_GETTIME)
#define CLOCK_HAS_GETTIME
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res) && (CLOCK_REALTIME == clk_id)) {
        int err = 0;
        struct timeval tv;
        if ((err = gettimeofday(&tv, NULL))) {
            res->tv_sec = 0;
            res->tv_nsec = 0;
            return errno;
        } else {
            res->tv_sec = tv.tv_sec;
            res->tv_nsec = ::xos::useconds_nseconds(tv.tv_usec);
            return 0;
        }
    }
    return EINVAL;
}
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// !defined(CLOCK_REALTIME)

namespace xos {
namespace platform {
namespace os {
namespace oracle {
namespace solaris {

typedef mt::os::semaphore semaphore;

} /// namespace solaris
} /// namespace oracle
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(SOLARIS)
///
/// solaris semaphores
/// ...
int sema_init(sema_t *sp, unsigned int count, int type, void *arg) {
    if ((sp)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        int err = EACCES;
        try {
            if ((semaphore = new ::xos::platform::os::oracle::solaris::semaphore(0, false, false))) {
                if ((semaphore->create())) {
                    *sp = ((sema_t)semaphore);
                    return 0;
                } else {
                    err = EFAULT;
                }
                delete semaphore;
            }
        } catch (...) {
            err = ENOMEM;
        }
        return err;
    }
    return EINVAL;
}   
int sema_destroy(sema_t *sp) {
    if ((sp)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            int err = EACCES;
            if ((semaphore->destroy())) {
                err = 0;
            }
            try {
                *sp = 0;
                delete semaphore;
            } catch (...) {
                err = ENOMEM;
            }
            return err;
        }
    }
    return EINVAL;
}   
int sema_post(sema_t *sp) {
    if ((sp)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            if ((semaphore->release())) {
                return 0;
            }
        }
    }
    return EINVAL;
}   
int sema_wait(sema_t *sp) {
    if ((sp)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            if ((semaphore->acquire())) {
                return 0;
            }
        }
    }
    return EINVAL;
}   
int sema_trywait(sema_t *sp) {
    if ((sp)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            ::xos::acquire_status status = ::xos::acquire_failed;
            if (::xos::acquire_success == (status = semaphore->try_acquire())) {
                return 0;
            } else {
                if (::xos::acquire_busy == (status)) {
                    return EBUSY;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        return ETIMEDOUT;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            return EINTR;
                        } else {
                            return EFAULT;
                        }
                    }
                }
            }
        }
    }
    return EINVAL;
}   
int sema_timedwait(sema_t *sp, timestruc_t *abstime) {
    if ((sp) && (abstime)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            mseconds_t abs_milliseconds = ::xos::seconds_mseconds(abstime->tv_sec) + ::xos::nseconds_mseconds(abstime->tv_nsec);
            int err = EFAULT;
            struct timespec timeout;
            if (!(err = clock_gettime(CLOCK_REALTIME, &timeout))) {
                mseconds_t milliseconds = ::xos::seconds_mseconds(timeout.tv_sec) + ::xos::nseconds_mseconds(timeout.tv_nsec);
                ::xos::acquire_status status = ::xos::acquire_failed;
                if (::xos::acquire_success == (status = semaphore->timed_acquire((abs_milliseconds >= milliseconds)?(abs_milliseconds - milliseconds):(0)))) {
                    return 0;
                } else {
                    if (::xos::acquire_busy == (status)) {
                        return EBUSY;
                    } else {
                        if (::xos::acquire_timeout == (status)) {
                            return ETIMEDOUT;
                        } else {
                            if (::xos::acquire_interrupted == (status)) {
                                return EINTR;
                            } else {
                                return EFAULT;
                            }
                        }
                    }
                }
            }
        }
    }
    return EINVAL;
}   
int sema_reltimedwait(sema_t *sp, timestruc_t *reltime) {
    if ((sp) && (reltime)) {
        ::xos::platform::os::oracle::solaris::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::oracle::solaris::semaphore*)(*sp)))) {
            mseconds_t milliseconds = ::xos::seconds_mseconds(reltime->tv_sec) + ::xos::nseconds_mseconds(reltime->tv_nsec);
            ::xos::acquire_status status = ::xos::acquire_failed;
            if (::xos::acquire_success == (status = semaphore->timed_acquire(milliseconds))) {
                return 0;
            } else {
                if (::xos::acquire_busy == (status)) {
                    return EBUSY;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        return ETIMEDOUT;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            return EINTR;
                        } else {
                            return EFAULT;
                        }
                    }
                }
            }
        }
    }
    return EINVAL;
}   
/// ...
/// solaris semaphores
/// 
#endif /// !defined(SOLARIS)
