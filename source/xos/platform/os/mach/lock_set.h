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
///   File: lock_set.h
///
/// Author: $author$
///   Date: 12/31/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_OS_MACH_LOCK_SET_H
#define XOS_PLATFORM_OS_MACH_LOCK_SET_H

#include "xos/platform/os.h"

#if !defined(MACH) || defined(APPLE)
#if !defined(APPLE)
#include "xos/platform/os/mach/task.h"
typedef mach_port_t lock_set_t;
#else /// !defined(APPLE)
#define lock_set_create platform_lock_set_create
#define lock_set_destroy platform_lock_set_destroy
#define lock_acquire platform_lock_acquire
#define lock_release platform_lock_release
#define lock_try platform_lock_try
#define lock_set_t platform_lock_set_t
typedef void* lock_set_t;
#endif /// !defined(APPLE)
#endif /// !defined(MACH) || defined(APPLE)

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

#if !defined(MACH) || defined(APPLE)
///
/// mach lock_set
/// ...
kern_return_t lock_set_create (
        task_t task,
        lock_set_t *new_lock_set,
        int n_ulocks,
        int policy
);

kern_return_t lock_set_destroy (
        task_t task,
        lock_set_t lock_set
);

kern_return_t lock_acquire (
        lock_set_t lock_set,
        int lock_id
);

kern_return_t lock_release (
        lock_set_t lock_set,
        int lock_id
);

kern_return_t lock_try (
        lock_set_t lock_set,
        int lock_id
);
/// ...
/// mach lock_set
///
#endif /// !defined(MACH) || defined(APPLE)

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_OS_MACH_LOCK_SET_H /*/
