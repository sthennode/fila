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
///   File: mutex.h
///
/// Author: $author$
///   Date: 12/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_OS_ORACLE_SOLARIS_MUTEX_H
#define XOS_PLATFORM_OS_ORACLE_SOLARIS_MUTEX_H

#include "xos/platform/os/oracle/solaris/sync.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

/*/
/// enum lock_t
/*/
typedef int lock_t;
enum {
    LOCK_ROBUST,
    LOCK_RECURSIVE,
    LOCK_ERRORCHECK,
    LOCK_PRIO_INHERIT,
    LOCK_PRIO_PROTECT
}; /*/ enum lock_t /*/

typedef void* mutex_t;
extern int mutex_init(mutex_t *mp, int type, void *arg);
extern int mutex_destroy(mutex_t *mp);
extern int mutex_lock(mutex_t *mp);
extern int mutex_unlock(mutex_t *mp);
extern int mutex_trylock(mutex_t *mp);
extern int mutex_timedlock(mutex_t *mp, timestruc_t *abstime);
extern int mutex_reltimedlock(mutex_t *mp, timestruc_t *reltime);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_OS_ORACLE_SOLARIS_MUTEX_H /*/
