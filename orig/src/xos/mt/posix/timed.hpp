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
///   File: timed.hpp
///
/// Author: $author$
///   Date: 4/12/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_TIMED_HPP
#define _XOS_MT_POSIX_TIMED_HPP

#include "xos/base/base.hpp"

#include <time.h>
#include <errno.h>

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#if !defined(HAS_CLOCK_GETTIME)
#define HAS_CLOCK_GETTIME
#endif // !defined(HAS_CLOCK_GETTIME)  
#else // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
//
// clock_gettime
//
#if !defined(HAS_CLOCK_GETTIME)
#define HAS_CLOCK_GETTIME
#if !defined(clock_gettime)
#define clock_gettime(f, t) memset(t, 0, sizeof(t))
#endif // !defined(clock_gettime)
#endif // !defined(HAS_CLOCK_GETTIME)  
#endif // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
/// Function: timed_until_time
///////////////////////////////////////////////////////////////////////
inline struct timespec timed_until_time(mseconds_t milliseconds) {
    struct timespec t;
#if defined(HAS_CLOCK_GETTIME)  
    clock_gettime(CLOCK_REALTIME, &t);
#else // defined(HAS_CLOCK_GETTIME)  
    memset(t, 0, sizeof(t));
#endif // defined(HAS_CLOCK_GETTIME)  
    t.tv_sec +=  mseconds_seconds(milliseconds);
    t.tv_nsec +=  mseconds_nseconds(milliseconds);
    return t;
}

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_TIMED_HPP 
        

