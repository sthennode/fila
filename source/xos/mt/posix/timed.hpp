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
///   File: timed.hpp
///
/// Author: $author$
///   Date: 12/25/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_POSIX_TIMED_HPP
#define XOS_MT_POSIX_TIMED_HPP

#include "xos/base/time.hpp"

#include <sys/time.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

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
        int err = EFAULT;
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

///
/// clock_gettime_relative_np
///
#if !defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
#define CLOCK_HAS_GETTIME_RELATIVE_NP
inline int clock_gettime_relative_np(struct timespec *timeout, const struct timespec *relative_timeout) {
    if ((timeout) && (relative_timeout)) {
        int err = EFAULT;
        struct timespec until_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            seconds_t seconds = until_time.tv_sec + relative_timeout->tv_sec;
            nseconds_t nseconds = until_time.tv_nsec + relative_timeout->tv_nsec;
            timeout->tv_sec = (seconds + ::xos::nseconds_seconds(nseconds));
            timeout->tv_nsec = ::xos::nseconds_nseconds(nseconds);
            return err;
        }
    }
    return EINVAL;
}
#endif /// !defined(CLOCK_HAS_GETTIME_RELATIVE_NP)

namespace xos {
namespace mt {
namespace posix {

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_POSIX_TIMED_HPP
