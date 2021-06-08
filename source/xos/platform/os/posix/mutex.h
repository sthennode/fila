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
///   Date: 1/25/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_OS_POSIX_MUTEX_H
#define XOS_PLATFORM_OS_POSIX_MUTEX_H

#include "xos/platform/os/posix/sys/time.h"

#if !defined(WINDOWS)
/*/
/// posix mutexes
/// ...
/*/
#define pthread_mutex_t platform_pthread_mutex_t
#define pthread_mutex_init platform_pthread_mutex_init
#define pthread_mutex_destroy platform_pthread_mutex_destroy

#define pthread_mutex_lock platform_pthread_mutex_lock
#define pthread_mutex_trylock platform_pthread_mutex_trylock
#define pthread_mutex_timedlock platform_pthread_mutex_timedlock
#define pthread_mutex_timedlock_relative_np platform_pthread_mutex_timedlock_relative_np
#define pthread_mutex_unlock platform_pthread_mutex_unlock

#define pthread_mutexattr_t platform_pthread_mutexattr_t
#define pthread_mutexattr_init platform_pthread_mutexattr_init
#define pthread_mutexattr_destroy platform_pthread_mutexattr_destroy
#define pthread_mutexattr_gettype platform_pthread_mutexattr_gettype
#define pthread_mutexattr_settype platform_pthread_mutexattr_settype
#define pthread_mutexattr_getpshared platform_pthread_mutexattr_getpshared
#define pthread_mutexattr_setpshared platform_pthread_mutexattr_setpshared
/*/
/// ...
/// posix mutexes
/*/
#endif /// !defined(WINDOWS)

#if !defined(PTHREAD_MUTEX_HAS_TRYLOCK)
#define PTHREAD_MUTEX_HAS_TRYLOCK
#endif // !defined(PTHREAD_MUTEX_HAS_TRYLOCK)

#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#endif // !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)

#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
#define PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP
#endif // !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

/*/
/// posix mutexes
/// ...
/*/
typedef pointer_t pthread_mutex_t;
typedef pointer_t pthread_mutexattr_t;

extern int pthread_mutex_init (pthread_mutex_t  *mutex, const pthread_mutexattr_t  *attr);
extern int pthread_mutex_destroy (pthread_mutex_t  *mutex);

extern int pthread_mutex_lock (pthread_mutex_t  *mutex);
extern int pthread_mutex_trylock (pthread_mutex_t  *mutex);
extern int pthread_mutex_timedlock (pthread_mutex_t *mutex, const struct timespec *abstime);
extern int pthread_mutex_timedlock_relative_np (pthread_mutex_t *mutex, const struct timespec *reltime);
extern int pthread_mutex_unlock (pthread_mutex_t  *mutex);

extern int pthread_mutexattr_init (pthread_mutexattr_t *attr);
extern int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);
extern int pthread_mutexattr_gettype (const pthread_mutexattr_t *attr, int *type);
extern int pthread_mutexattr_settype (pthread_mutexattr_t *attr, int type);
extern int pthread_mutexattr_getpshared (const pthread_mutexattr_t *attr, int *pshared);
extern int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int pshared);
/*/
/// ...
/// posix mutexes
/*/

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ XOS_PLATFORM_OS_POSIX_MUTEX_H /*/
