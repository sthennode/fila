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
///   File: condition.h
///
/// Author: $author$
///   Date: 1/15/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_CONDITION_H
#define XOS_PLATFORM_CONDITION_H

#include "xos/platform/os.h"

typedef pointer_t platform_condition_t;

/*/ enum platform_condition_attr_t /*/
typedef pointer_t platform_condition_attr_t;
typedef int platform_condition_attr_none_t;
enum {
    platform_condition_attr_none = 0
}; /*/ enum platform_condition_attr_t /*/

/*/ enum platform_condition_error_t /*/ 
typedef int platform_condition_error_t;
enum {
    platform_condition_error_success = 0,
    platform_condition_error_failed,
    platform_condition_error_busy,
    platform_condition_error_timeout,
    platform_condition_error_interrupted
}; /*/ enum platform_condition_error_t /*/

/*/ struct platform_condition_timeout /*/
typedef struct platform_condition_timeout platform_condition_timeout_t;
struct platform_condition_timeout {
    seconds_t tv_sec;
    nseconds_t tv_nsec;    
}; /*/ struct platform_condition_timeout /*/

extern platform_condition_error_t platform_condition_create(platform_condition_t* condition, platform_condition_attr_t attr);
extern platform_condition_error_t platform_condition_destroy(platform_condition_t* condition);
extern platform_condition_error_t platform_condition_wait(platform_condition_t* condition);
extern platform_condition_error_t platform_condition_time_wait(platform_condition_t* condition, platform_condition_timeout_t* timeout);
extern platform_condition_error_t platform_condition_timed_wait(platform_condition_t* condition, platform_condition_timeout_t* timeout);
extern platform_condition_error_t platform_condition_try_wait(platform_condition_t* condition);
extern platform_condition_error_t platform_condition_signal(platform_condition_t* condition);

#endif /*/ ndef XOS_PLATFORM_CONDITION_H /*/
