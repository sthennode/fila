/*/
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
///   File: mutex.h
///
/// Author: $author$
///   Date: 1/1/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_MUTEX_H
#define XOS_PLATFORM_MUTEX_H

#include "xos/platform/os.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

typedef pointer_t platform_mutex_t;

/*/ enum platform_mutex_attr_t /*/
typedef int platform_mutex_attr_t;
enum {
    platform_mutex_attr_none = 0
}; /*/ enum platform_mutex_attr_t /*/

/*/ enum platform_mutex_error_t /*/ 
typedef int platform_mutex_error_t;
enum {
    platform_mutex_error_success = 0,
    platform_mutex_error_failed,
    platform_mutex_error_busy,
    platform_mutex_error_timeout,
    platform_mutex_error_interrupted
}; /*/ enum platform_mutex_error_t /*/

/*/ struct platform_mutex_timeout /*/
typedef struct platform_mutex_timeout platform_mutex_timeout_t;
struct platform_mutex_timeout {
    seconds_t tv_sec;
    nseconds_t tv_nsec;    
}; /*/ struct platform_mutex_timeout /*/

extern platform_mutex_error_t platform_mutex_create(platform_mutex_t* mutex, platform_mutex_attr_t attr);
extern platform_mutex_error_t platform_mutex_destroy(platform_mutex_t* mutex);
extern platform_mutex_error_t platform_mutex_lock(platform_mutex_t* mutex);
extern platform_mutex_error_t platform_mutex_timedlock(platform_mutex_t* mutex, platform_mutex_timeout_t* timeout);
extern platform_mutex_error_t platform_mutex_trylock(platform_mutex_t* mutex);
extern platform_mutex_error_t platform_mutex_unlock(platform_mutex_t* mutex);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_MUTEX_H /*/
