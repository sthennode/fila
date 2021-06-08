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
///   Date: 1/7/2020
///////////////////////////////////////////////////////////////////////
/*/
#if !defined(XOS_PLATFORM_OS_POSIX_SEMAPHORE_H) || defined(PLATFORM_OS_POSIX_SEMAPHORE)
#if !defined(XOS_PLATFORM_OS_POSIX_SEMAPHORE_H) && !defined(PLATFORM_OS_POSIX_SEMAPHORE)
#define XOS_PLATFORM_OS_POSIX_SEMAPHORE_H
#endif /*/ !defined(XOS_PLATFORM_OS_POSIX_SEMAPHORE_H) && !defined(PLATFORM_OS_POSIX_SEMAPHORE) /*/

#include "xos/platform/os/posix/sys/time.h"

#if !defined(WINDOWS) || defined(PLATFORM_OS_POSIX_SEMAPHORE)
/*/
/// posix semaphores
/// ...
/*/
#if defined(PLATFORM_OS_POSIX_SEMAPHORE)
typedef void* platform_posix_semaphore_t;
#define platform_posix_sem_t platform_posix_semaphore_t
#define sem_t platform_posix_sem_t

#define sem_init platform_posix_sem_init
#define sem_destroy platform_posix_sem_destroy

#define sem_open platform_posix_sem_open
#define sem_close platform_posix_sem_close
#define sem_unlink platform_posix_sem_unlink

#define sem_post platform_posix_sem_post
#define sem_wait platform_posix_sem_wait
#define sem_trywait platform_posix_sem_trywait
#define sem_timedwait platform_posix_sem_timedwait
#define sem_timedwait_relative_np platform_posix_sem_timedwait_relative_np
#else defined(PLATFORM_OS_POSIX_SEMAPHORE)
typedef void* posix_semaphore_t;
#define posix_sem_t posix_semaphore_t
#define sem_t posix_sem_t

#define sem_init posix_sem_init
#define sem_destroy posix_sem_destroy

#define sem_open posix_sem_open
#define sem_close posix_sem_close
#define sem_unlink posix_sem_unlink

#define sem_post posix_sem_post
#define sem_wait posix_sem_wait
#define sem_trywait posix_sem_trywait
#define sem_timedwait posix_sem_timedwait
#define sem_timedwait_relative_np posix_sem_timedwait_relative_np
#endif /// defined(PLATFORM_OS_POSIX_SEMAPHORE)
/*/
/// ...
/// posix semaphores
/*/
#endif /// !defined(WINDOWS) || defined(PLATFORM_OS_POSIX_SEMAPHORE)

#if !defined(POSIX_SEM_HAS_TRYWAIT)
#define POSIX_SEM_HAS_TRYWAIT
#endif // !defined(POSIX_SEM_HAS_TRYWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif // !defined(POSIX_SEM_HAS_TIMEDWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif // !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

extern int sem_init(sem_t *sem, int pshared, unsigned int value);
extern int sem_destroy(sem_t *sem);

extern sem_t *sem_open(const char *name, int oflag, ...);
extern int sem_close(sem_t *sem);
extern int sem_unlink(const char *name);

extern int sem_post(sem_t *sem);
extern int sem_wait(sem_t *sem);
extern int sem_trywait(sem_t *sem);
extern int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
extern int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ !defined(XOS_PLATFORM_OS_POSIX_SEMAPHORE_H) || defined(PLATFORM_OS_POSIX_SEMAPHORE) /*/

