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
///   Date: 1/12/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_OS_MACH_SEMAPHORE_H
#define XOS_PLATFORM_OS_MACH_SEMAPHORE_H

#include "xos/platform/os/mach/task.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

#if !defined(MACH)
/*/
/// mach semaphores
/// ...
/*/
typedef mach_port_t semaphore_t;

extern kern_return_t semaphore_create(task_t task, semaphore_t *semaphore, int policy, int value);
extern kern_return_t semaphore_destroy(task_t task, semaphore_t semaphore);

extern kern_return_t semaphore_signal(semaphore_t semaphore);
extern kern_return_t semaphore_wait(semaphore_t semaphore);
extern kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time);
/*/
/// ...
/// mach semaphores
/*/
#endif /*/ !defined(MACH) /*/

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_OS_MACH_SEMAPHORE_H /*/
