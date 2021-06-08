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
///   File: semaphore.h
///
/// Author: $author$
///   Date: 1/5/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_SEMAPHORE_H
#define XOS_PLATFORM_SEMAPHORE_H

#include "xos/platform/os.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

typedef pointer_t platform_semaphore_t;

/*/ enum platform_semaphore_attr_t /*/
typedef int platform_semaphore_attr_t;
enum {
    platform_semaphore_attr_none = 0
}; /*/ enum platform_semaphore_attr_t /*/

/*/ enum platform_semaphore_error_t /*/ 
typedef int platform_semaphore_error_t;
enum {
    platform_semaphore_error_success = 0,
    platform_semaphore_error_failed,
    platform_semaphore_error_busy,
    platform_semaphore_error_timeout,
    platform_semaphore_error_interrupted
}; /*/ enum platform_semaphore_error_t /*/

/*/ struct platform_semaphore_timeout /*/
typedef struct platform_semaphore_timeout platform_semaphore_timeout_t;
struct platform_semaphore_timeout {
    seconds_t tv_sec;
    nseconds_t tv_nsec;    
}; /*/ struct platform_semaphore_timeout /*/

extern platform_semaphore_error_t platform_semaphore_create(platform_semaphore_t* semaphore, platform_semaphore_attr_t attr);
extern platform_semaphore_error_t platform_semaphore_destroy(platform_semaphore_t* semaphore);
extern platform_semaphore_error_t platform_semaphore_acquire(platform_semaphore_t* semaphore);
extern platform_semaphore_error_t platform_semaphore_timed_acquire(platform_semaphore_t* semaphore, platform_semaphore_timeout_t* timeout);
extern platform_semaphore_error_t platform_semaphore_try_acquire(platform_semaphore_t* semaphore);
extern platform_semaphore_error_t platform_semaphore_release(platform_semaphore_t* semaphore);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_SEMAPHORE_H /*/
