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
///   Date: 1/9/2020
///////////////////////////////////////////////////////////////////////
/*/
#ifndef XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H
#define XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H

#include "xos/platform/os/oracle/solaris/sync.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

typedef void* sema_t;
extern int sema_init(sema_t *sp, unsigned int count, int type, void *arg);
extern int sema_destroy(sema_t *sp);
extern int sema_post(sema_t *sp);
extern int sema_wait(sema_t *sp);
extern int sema_trywait(sema_t *sp);
extern int sema_timedwait(sema_t *sp, timestruc_t *abstime);
extern int sema_reltimedwait(sema_t *sp, timestruc_t *reltime);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ ndef XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H /*/
