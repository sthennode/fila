/*/
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
///   File: thread.h
///
/// Author: $author$
///   Date: 1/20/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_THREAD_H
#define XOS_PLATFORM_THREAD_H

#include "xos/platform/os.h"

typedef pointer_t platform_thread_t;
typedef pointer_t platform_thread_attr_t;
typedef pointer_t (*platform_thread_start_routine_t)(pointer_t);

/*/ enum platform_thread_attr_none_t /*/
typedef int platform_thread_attr_none_t;
enum {
    platform_thread_attr_none = 0
}; /*/ enum platform_thread_attr_none_t /*/

/*/ enum platform_thread_error_t /*/ 
typedef int platform_thread_error_t;
enum {
    platform_thread_error_success = 0,
    platform_thread_error_failed,
    platform_thread_error_busy,
    platform_thread_error_timeout,
    platform_thread_error_interrupted
}; /*/ enum platform_thread_error_t /*/

/*/ struct platform_thread_timeout /*/
typedef struct platform_thread_timeout platform_thread_timeout_t;
struct platform_thread_timeout {
    seconds_t tv_sec;
    nseconds_t tv_nsec;    
}; /*/ struct platform_thread_timeout /*/

extern platform_thread_error_t platform_thread_create(platform_thread_t* thread, platform_thread_attr_t attr, platform_thread_start_routine_t start_routine);
extern platform_thread_error_t platform_thread_destroy(platform_thread_t* thread);
extern platform_thread_error_t platform_thread_join(platform_thread_t* thread);
extern platform_thread_error_t platform_thread_time_join(platform_thread_t* thread, platform_thread_timeout_t* timeout);
extern platform_thread_error_t platform_thread_timed_join(platform_thread_t* thread, platform_thread_timeout_t* timeout);
extern platform_thread_error_t platform_thread_try_join(platform_thread_t* thread);

#endif /*/ ndef XOS_PLATFORM_THREAD_H /*/
