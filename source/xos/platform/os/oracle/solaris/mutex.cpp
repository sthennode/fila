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
///   File: mutex.cpp
///
/// Author: $author$
///   Date: 12/27/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/oracle/solaris/mutex.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace platform {
namespace os {
namespace oracle {
namespace solaris {

} /// namespace solaris
} /// namespace oracle
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(SOLARIS)
///
/// solaris mutexes
/// ...
int mutex_init(mutex_t *mp, int type, void *arg) {
    if ((mp)) {
        int err = EFAULT;
        try {
            ::xos::mt::os::mutex* mtx = 0;
            if ((mtx = new ::xos::mt::os::mutex(((::xos::mt::os::mutex::attached_t)::xos::mt::os::mutex::unattached), false, false))) {
                if ((mtx->create())) {
                    *mp = mtx;
                    err = 0;
                } else {
                    delete mtx;
                }
            }
        } catch (...) {
            err = ENOMEM;
        }
        return err;
    }
    return EINVAL;
}
int mutex_destroy(mutex_t *mp) {
    if ((mp)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            int err = EFAULT;
            if ((mtx->destroy())) {
                err = 0;
            }
            *mp = 0;
            try {
                delete mtx;
            } catch (...) {
                err = ENOMEM;
            }
            return err;
        }
    }
    return EINVAL;
}
int mutex_lock(mutex_t *mp) {
    if ((mp)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            if ((mtx->lock())) {
                return 0;
            }
            return EFAULT;
        }
    }
    return EINVAL;
}
int mutex_trylock(mutex_t *mp) {
    if ((mp)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            ::xos::lock_status status = mtx->try_lock();
            if (::xos::lock_success == (status)) {
                return 0;
            } else {
                if (::xos::lock_busy == (status)) {
                    return EBUSY;
                } else {
                    if (::xos::lock_timeout == (status)) {
                        return ETIMEDOUT;
                    } else {
                        if (::xos::lock_interrupted == (status)) {
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
int mutex_unlock(mutex_t *mp) {
    if ((mp)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            if ((mtx->unlock())) {
                return 0;
            }
            return EFAULT;
        }
    }
    return EINVAL;
}
int mutex_timedlock(mutex_t *mp, timestruc_t *abstime) {
    if ((mp) && (abstime)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            int err = 0;
            timestruc_t reltime;
            if (!(err = ::clock_gettime(CLOCK_REALTIME, &reltime))) {
                reltime.tv_sec = abstime->tv_sec - reltime.tv_sec;
                reltime.tv_nsec = abstime->tv_nsec - reltime.tv_nsec;
                err = mutex_reltimedlock(mp, &reltime);
            }
            return err;
        }
    }
    return EINVAL;
}
int mutex_reltimedlock(mutex_t *mp, timestruc_t *reltime) {
    if ((mp) && (reltime)) {
        ::xos::mt::os::mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::mutex*)(*mp)))) {
            mseconds_t milliseconds = ::xos::seconds_mseconds(reltime->tv_sec) + ::xos::nseconds_mseconds(reltime->tv_nsec);
            ::xos::lock_status status = mtx->timed_lock(milliseconds);
            if (::xos::lock_success == (status)) {
                return 0;
            } else {
                if (::xos::lock_busy == (status)) {
                    return EBUSY;
                } else {
                    if (::xos::lock_timeout == (status)) {
                        return ETIMEDOUT;
                    } else {
                        if (::xos::lock_interrupted == (status)) {
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
/// solaris mutexes
/// 
#endif /// !defined(SOLARIS)
