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
///   File: condition.hpp
///
/// Author: $author$
///   Date: 1/15/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_POSIX_CONDITION_HPP
#define XOS_MT_POSIX_CONDITION_HPP

#include "xos/mt/condition.hpp"

#if !defined(WINDOWS)
#include "xos/mt/posix/timed.hpp"
#include "xos/mt/posix/mutex.hpp"
#include <pthread.h>
#include <time.h>
#include <errno.h>
#else /// !defined(WINDOWS)
#include "xos/platform/os/posix/condition.h"
#endif /// !defined(WINDOWS)

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#define PTHREAD_COND_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if defined(APPLEOSx)
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#define PTHREAD_COND_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(APPLEOS)

///
/// pthread_cond_timedwait
/// 
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT)
inline int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abs_timeout) {
    if ((cond) && (mutex) && (abs_timeout)) {
        int err = EFAULT; 
        struct timespec abs_time;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &abs_time))) {
            useconds_t time_useconds = ::xos::seconds_useconds(abs_time.tv_sec) + ::xos::nseconds_useconds(abs_time.tv_nsec),
                       timeout_useconds = ::xos::seconds_useconds(abs_timeout->tv_sec) + ::xos::nseconds_useconds(abs_timeout->tv_nsec);
            if (timeout_useconds > time_useconds) {
                for (useconds_t useconds_sleep = ::xos::mseconds_useconds(10);
                     timeout_useconds > time_useconds; time_useconds += useconds_sleep) {
#if defined(PTHREAD_COND_HAS_TRYWAIT)
                    err = ::pthread_cond_trywait(cond, mutex);
#else /// defined(PTHREAD_COND_HAS_TRYWAIT)
                    err = EAGAIN;
#endif /// defined(PTHREAD_COND_HAS_TRYWAIT)
                    if (EAGAIN != (err)) {
                        if (EBUSY != (err)) {
                            if (ETIMEDOUT != (err)) {
                                return err;
                            }
                        }
                    }
                    ::usleep(useconds_sleep);
                }
            }
            return ETIMEDOUT;
        }
    }
    return EINVAL; 
}
#define PTHREAD_COND_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT)

///
/// pthread_cond_timedwait_relative_np
/// 
#if !defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
inline int pthread_cond_timedwait_relative_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* reltime) {
#if defined(PTHREAD_COND_HAS_TIMEDWAIT)
    if ((cond) && (mutex) && (reltime)) {
        int err = EFAULT; 
        struct timespec until_time;
        if (!(err = ::clock_gettime_relative_np(&until_time, reltime))) {
            err = ::pthread_cond_timedwait(cond, mutex, &until_time);
        }
        return err;
    }
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
    return EINVAL; 
}
#define PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// !defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)

///
/// pthread_cond_trywait
/// 
#if !defined(PTHREAD_COND_HAS_TRYWAIT)
inline int pthread_cond_trywait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    if ((cond) && (mutex)) {
        int err = EBUSY; 
        struct timespec until_time;
#if defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        until_time.tv_sec = 0;
        until_time.tv_nsec = 0;
        err = ::pthread_cond_timedwait_relative_np(cond, mutex, &until_time);
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
#if defined(PTHREAD_COND_HAS_TIMEDWAIT)
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
            err = ::pthread_cond_timedwait(cond, mutex, &until_time);
        }
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        return err;
    }
    return EINVAL;
}
#define PTHREAD_COND_HAS_TRYWAIT
#endif /// !defined(PTHREAD_COND_HAS_TRYWAIT)

namespace xos {
namespace mt {
namespace posix {

/// class conditiont
template 
<//class TExtends = mt::extended::conditiont<mutex, pthread_cond_t*>, 
 class TExtends = mt::derive::conditiont
 <mutex, pthread_cond_t, int, 0, -1, EBUSY, ETIMEDOUT, EINTR>, 
 class TImplements = typename TExtends::implements>
class exported conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef conditiont derives;
    
    typedef typename extends::wait_detached_t wait_detached_t;
    typedef typename extends::cond_t cond_t;
    typedef typename extends::mutex_t mutex_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

/*/
    typedef signal_status (derives::*wait_detached_t)(pthread_cond_t&, pthread_mutex_t&, mseconds_t) const;

    /// constructor / destructor
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached), signaled_(&signaled), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(is_logged), signaled_(&signaled), mutex_(mutex.attached_to()) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(mutex_t& mutex, bool& signaled): signaled_(&signaled), mutex_(mutex.attached_to()) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(0), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created): extends(detached, is_created), signaled_(0), mutex_(mutex.attached_to()) {
    }
    conditiont(mutex_t& mutex, attached_t detached): extends(detached), signaled_(0), mutex_(mutex.attached_to()) {
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached): extends(detached), signaled_(0), mutex_(0) {
    }
    conditiont(bool is_logged): extends(is_logged), signaled_(0), mutex_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(const conditiont& copy): extends(copy), signaled_(0), mutex_(0) {
    }
    conditiont(): signaled_(0), mutex_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~conditiont() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }
/*/
    /// constructor / destructor
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(mutex, signaled, detached, is_created, is_logged) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(mutex, signaled, detached, is_created) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(mutex, signaled, detached) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(mutex, signaled, is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(mutex_t& mutex, bool& signaled): extends(mutex, signaled) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created, bool is_logged): extends(mutex, detached, is_created, is_logged) {
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created): extends(mutex, detached, is_created) {
    }
    conditiont(mutex_t& mutex, attached_t detached): extends(mutex, detached) {
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(attached_t detached): extends(detached) {
    }
    conditiont(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(const conditiont& copy): extends(copy) {
    }
    conditiont() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~conditiont() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }
/*/
    /// ...wait... / ...signal...
    virtual wait_status time_wait(mseconds_t milliseconds) {
        return try_timed_untimed_wait(&derives::time_wait_detached, milliseconds);
    }
    virtual wait_status timed_wait(mseconds_t milliseconds) {
        return try_timed_untimed_wait(&derives::timed_wait_detached, milliseconds);
    }
    virtual wait_status untimed_wait() {
        return try_timed_untimed_wait(&derives::untimed_wait_detached, -1);
    }
    virtual wait_status try_wait() {
        return try_timed_untimed_wait(&derives::try_wait_detached, 0);
    }
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, mseconds_t milliseconds) { 
        signal_status status = wait_failed;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            pthread_mutex_t* mutex = 0;
            if (((mutex = mutex_))) {
                bool* signaled = 0;
                if (((signaled = signaled_))) {
                    status = try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    status = try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, milliseconds);
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
            }
        }
        return status; 
    }
    virtual bool signal() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            bool success = false;
            pthread_mutex_t* mutex = 0;
            if ((0 != (mutex = mutex_))) {
                bool* signaled = 0;
                if ((0 != (signaled = signaled_))) {
                    success = signal_detached(*detached, *mutex, *signaled);
                } else {
                    success = signal_detached(*detached);
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
            }
            return success;
        }
        return false;
    }
/*/
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, mseconds_t milliseconds) { 
        signal_status status = wait_failed;
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = this->attached_to())) {
            mutex_t* mutex = 0;
            if (((mutex = this->mutex_))) {
                bool* signaled = 0;
                if (((signaled = this->signaled_))) {
                    status = this->try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    status = this->try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, milliseconds);
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
            }
        }
        return status; 
    }
    virtual bool signal() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            bool success = false;
            mutex_t* mutex = 0;
            if ((0 != (mutex = this->mutex_))) {
                bool* signaled = 0;
                if ((0 != (signaled = this->signaled_))) {
                    success = this->signal_detached(*detached, *mutex, *signaled);
                } else {
                    success = this->signal_detached(*detached);
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
            }
            return success;
        }
        return false;
    }
/*/
    /// ...wait_detached... / ...signal_detached...
    virtual signal_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        signal_status status = wait_failed;
        if ((0 != (wait_detached))) {
            for (bool unsignaled = true; unsignaled; ) {
                try {
                    mutex_t locked(&mutex);
                    ::xos::lock lock(locked);
                    if (!(unsignaled = !signaled)) {
                        status = wait_success;
                    } else {
                        unsignaled = false;
                        lock.set_unlocked();
                        if (wait_success != (status = (this->*wait_detached)(cond, mutex, milliseconds))) {
                            lock.set_unlocked(false);
                        } else {
                            if (!(signaled)) {
                                LOGGER_IS_LOGGED_TRACE("...spurious wait_success");
                            }
                        }
                        if (!(signaled)) {
                            status = wait_busy;
                            if (0 > (milliseconds)) {
                                unsignaled = true;
                            }
                        }
                    }
                } catch (...) {
                    LOGGER_IS_LOGGED_ERROR("...catch(...)");
                    unsignaled = false;
                }
            }
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return status; 
    }
    virtual signal_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        signal_status status = wait_failed;
        if ((0 != (wait_detached))) {
            status = (this->*wait_detached)(cond, mutex, milliseconds);
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return status; 
    }
    virtual bool wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        return (wait_success == (untimed_wait_detached(cond, mutex, milliseconds))); 
    }
/*/
    virtual wait_status time_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        pthread_mutex_t* detached = 0;
        if ((detached = mutex.attached_to())) {
            return time_wait_detached(cond, *detached, milliseconds);
        }
        return wait_failed; 
    }
    virtual wait_status time_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
#if defined(PTHREAD_COND_HAS_TIMEDWAIT)
        int err = EFAULT;
        struct timespec until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IS_LOGGED_TRACE("::pthread_cond_timedwait(&cond, &mutex, &until_time)...");
        if ((err = ::pthread_cond_timedwait(&cond, &mutex, &until_time))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &until_time)");
                return wait_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &until_time)");
                return wait_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &until_time)");
                return wait_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &until_time)");
                return wait_failed;
            }
        } else {
            LOGGER_IS_LOGGED_TRACE("...::pthread_cond_timedwait(&cond, &mutex, &until_time)");
            return wait_success;
        }
        return wait_failed; 
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
        LOGGER_IS_LOGGED_ERROR("...return wait_invalid...");
        return wait_invalid; 
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT)
    }
    virtual wait_status timed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        pthread_mutex_t* detached = 0;
        if ((detached = mutex.attached_to())) {
            return timed_wait_detached(cond, *detached, milliseconds);
        }
        return wait_failed; 
    }
    virtual wait_status timed_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
#if defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        bool debug = this->is_logged_debug(milliseconds), logged = this->is_logged();
        int err = EFAULT;
        struct timespec until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)...");
        if ((err = ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time))) {
            switch(err) {
            case EBUSY:
                LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...EBUSY err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_busy;
            case ETIMEDOUT:
                LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...ETIMEDOUT err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
                return wait_failed;
            }
        } else {
            LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...::pthread_cond_timedwait_relative_np(&cond, &mutex, &until_time)");
            return wait_success;
        }
        return wait_failed; 
#else /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
        LOGGER_IS_LOGGED_ERROR("...return wait_invalid...");
        return wait_invalid; 
#endif /// defined(PTHREAD_COND_HAS_TIMEDWAIT_RELATIVE_NP)
    }
    virtual wait_status try_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        pthread_mutex_t* detached = 0;
        if ((detached = mutex.attached_to())) {
            return try_wait_detached(cond, *detached, milliseconds);
        }
        return wait_failed; 
    }
    virtual wait_status try_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
#if defined(PTHREAD_COND_HAS_TRYWAIT)
        int err = EFAULT;
        LOGGER_IS_LOGGED_TRACE("::pthread_cond_trywait(&cond, &mutex)...");
        if ((err = ::pthread_cond_trywait(&cond, &mutex))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return wait_failed;
            }
        } else {
            LOGGER_IS_LOGGED_TRACE("...::pthread_cond_trywait(&cond, &mutex)");
            return wait_success;
        }
#else /// defined(PTHREAD_COND_HAS_TRYWAIT)
        LOGGER_IS_LOGGED_ERROR("...return wait_invalid...");
        return wait_invalid; 
#endif /// defined(PTHREAD_COND_HAS_TRYWAIT)
        return wait_failed; 
    }
    virtual wait_status untimed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        pthread_mutex_t* detached = 0;
        if ((detached = mutex.attached_to())) {
            return untimed_wait_detached(cond, *detached, milliseconds);
        }
        return wait_failed; 
    }
    virtual wait_status untimed_wait_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        int err = EFAULT;
        LOGGER_IS_LOGGED_DEBUG("::pthread_cond_wait(&cond, &mutex)...");
        if ((err = ::pthread_cond_wait(&cond, &mutex))) {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_ERROR("...EBUSY err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_ERROR("...ETIMEDOUT err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return wait_failed;
            }
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::pthread_cond_wait(&cond, &mutex)");
            return wait_success;
        }
        return wait_failed; 
    }
/*/
    virtual bool signal_detached(pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            mutex_t locked(&mutex);
            ::xos::lock lock(locked);
            signaled = true;
        } catch (...) {
            LOGGER_IS_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = signal_detached(cond);
        return success;
    }
/*/
    using extends::signal_detached;
    virtual bool signal_detached(pthread_cond_t& cond) const { 
        int err = EFAULT;
        LOGGER_IS_LOGGED_DEBUG("::pthread_cond_signal(&cond)...");
        if (!(err = ::pthread_cond_signal(&cond))) {
            LOGGER_IS_LOGGED_DEBUG("...::pthread_cond_signal(&cond)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_signal(&cond)");
        }
        return false; 
    }
/*/
    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(cond_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            if ((this->destroy_detached(*detached))) {
                return true;
            }
        }
        return false;
    }
/*/
    virtual attached_t create_detached(pthread_cond_t& cond) const {
        attached_t detached = (attached_t)(unattached);
        int err = EFAULT;
        pthread_condattr_t condattr;

        LOGGER_IS_LOGGED_DEBUG("::pthread_condattr_init(&condattr)...");
        if (!(err = ::pthread_condattr_init(&condattr))) {
            LOGGER_IS_LOGGED_DEBUG("...::pthread_condattr_init(&condattr)");
            LOGGER_IS_LOGGED_DEBUG("::pthread_cond_init(&cond, &condattr)...");
            if (!(err = ::pthread_cond_init(&cond, &condattr))) {
                LOGGER_IS_LOGGED_DEBUG("...::pthread_cond_init(&cond, &condattr)");
                detached = &cond;
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_init(&cond, &condattr)");
            }
            LOGGER_IS_LOGGED_DEBUG("::pthread_condattr_destroy(&condattr)...");
            if ((err = ::pthread_condattr_destroy(&condattr))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_destroy(&condattr)");
                detached = ((attached_t)unattached);
                LOGGER_IS_LOGGED_DEBUG("::pthread_cond_destroy(&cond)...");
                if ((err = ::pthread_cond_destroy(&cond))) {
                    LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_destroy(&cond)");
                } else {
                    LOGGER_IS_LOGGED_DEBUG("...::pthread_cond_destroy(&cond)");
                }
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::pthread_condattr_destroy(&condattr)");
            }
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_init(&condattr)");
        }
        return detached;
    }
    virtual bool destroy_detached(pthread_cond_t& cond) const {
        int err = EFAULT;
        LOGGER_IS_LOGGED_DEBUG("::pthread_cond_destroy(&cond)...");
        if (((err = ::pthread_cond_destroy(&cond)))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::pthread_cond_destroy(&cond)");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::pthread_cond_destroy(&cond)");
            return true;
        }
        return false;
    }
/*/
protected:
    bool* signaled_;
    pthread_mutex_t* mutex_;
    pthread_cond_t cond_;
/*/
}; /// class conditiont
typedef conditiont<> condition;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_POSIX_CONDITION_HPP
