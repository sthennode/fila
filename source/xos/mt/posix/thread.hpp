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
///   File: thread.hpp
///
/// Author: $author$
///   Date: 1/21/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_POSIX_THREAD_HPP
#define XOS_MT_POSIX_THREAD_HPP

#include "xos/mt/thread.hpp"
#include "xos/mt/posix/timed.hpp"

#if !defined(windows)
#include <pthread.h>
#include <time.h>
#include <errno.h>
#else /// !defined(windows)
#include "xos/platform/os/platform/thread.h"
#endif /// !defined(windows)

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_HAS_TRYJOIN_NP)
#define PTHREAD_HAS_TRYJOIN_NP
#endif /// !defined(PTHREAD_HAS_TRYJOIN_NP)
#if !defined(PTHREAD_HAS_TIMEDJOIN_NP)
#define PTHREAD_HAS_TIMEDJOIN_NP
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN_NP)
#endif /// defined(HAS_POSIX_TIMEOUTS)

///
/// pthread_timedjoin_np
///
#if !defined(PTHREAD_HAS_TIMEDJOIN_NP)
inline int pthread_timedjoin_np(pthread_t thread, void **retval, const struct timespec *abstime) {
    return EINVAL;
}
#define PTHREAD_HAS_TIMEDJOIN_NP
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN_NP)

///
/// pthread_timedjoin_relative_np
///
#if !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
inline int pthread_timedjoin_relative_np(pthread_t thread, void **retval, const struct timespec *reltime) {
#if defined(PTHREAD_HAS_TIMEDJOIN_NP)
    if ((reltime)) {
        int err = EFAULT; 
        struct timespec until_time;
        if (!(err = ::clock_gettime_relative_np(&until_time, reltime))) {
            err = ::pthread_timedjoin_np(thread, retval, &until_time);
        }
        return err;
    }
#endif //// defined(PTHREAD_HAS_TIMEDJOIN_NP)
    return EINVAL;
}
#define PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP
#endif /// !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)

///
/// pthread_tryjoin_np
///
#if !defined(PTHREAD_HAS_TRYJOIN_NP)
inline int pthread_tryjoin_np(pthread_t thread, void **retval) {
    int err = EINVAL; 
    struct timespec until_time;
#if defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
    until_time.tv_sec = 0;
    until_time.tv_nsec = 0;
    err = ::pthread_timedjoin_relative_np(thread, retval, &until_time);
#else /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
#if defined(PTHREAD_HAS_TIMEDJOIN_NP)
    if (!(err = ::clock_gettime(CLOCK_REALTIME, &until_time))) {
        err = ::pthread_timedjoin_np(thread, retval, &until_time);
    }
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_NP)
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
    return err;
}
#define PTHREAD_HAS_TRYJOIN_NP
#endif /// !defined(PTHREAD_HAS_TRYJOIN_NP)

namespace xos {
namespace mt {
namespace posix {

/// class threadt
template 
<typename TThread = pthread_t,
 class TExtends = mt::derive::threadt<TThread, int, 0, -1, EBUSY, ETIMEDOUT, EINTR>, 
 class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef TThread thread_t;
    typedef typename extends::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    threadt(ran_t &ran, attached_t detached, bool is_created, bool is_logged): extends(ran, detached, is_created, is_logged) {
    }
    threadt(ran_t &ran, attached_t detached, bool is_created): extends(ran, detached, is_created) {
    }
    threadt(ran_t &ran, attached_t detached): extends(ran, detached) {
    }
    threadt(ran_t &ran, bool is_logged): extends(ran, is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(ran_t &ran): extends(ran) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    /// constructor / destructor
    threadt(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    threadt(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    threadt(attached_t detached): extends(detached) {
    }
    threadt(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(const threadt& copy): extends(copy) {
    }
    threadt() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->joined() throw (join_exception(join_failed))...");
            throw join_exception(join_failed);
        }
    }

    /// ...join_detached... / ...fork_detached...
    virtual join_status time_join_detached(bool& is_forked, thread_t& _thread, mseconds_t milliseconds) const { 
#if defined(PTHREAD_HAS_TIMEDJOIN_NP)
        bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
        void* value = 0;
        int err = 0;
        struct timespec until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::pthread_timedjoin_np(_thread, &value, &until_time)...");
        if (!(err = ::pthread_timedjoin_np(_thread, &value, &until_time))) {
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::pthread_timedjoin_np(_thread, &value, &until_time)");
            is_forked = false;
            return join_success;
        } else {
            switch(err) {
            case EBUSY:
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...EBUSY err = " << err << " on ::pthread_timedjoin_np(_thread, &value, &until_time)");
                return join_busy;
            case ETIMEDOUT:
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...ETIMEDOUT err = " << err << " on ::pthread_timedjoin_np(_thread, &value, &until_time)");
                return join_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_timedjoin_np(_thread, &value, &until_time)");
                is_forked = false;
                return join_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_timedjoin_np(_thread, &value, &until_time)");
                is_forked = false;
            }
        }
#else /// defined(PTHREAD_HAS_TIMEDJOIN_NP)
        LOGGER_IS_LOGGED_ERROR("...invalid ::pthread_timedjoin_np(_thread, &value, &until_time)...");
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_NP)
        return join_failed; 
    }
    virtual join_status timed_join_detached(bool& is_forked, thread_t& _thread, mseconds_t milliseconds) const { 
#if defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
        bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
        void* value = 0;
        int err = 0;
        struct timespec until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::pthread_timedjoin_relative_np(_thread, &value, &until_time)...");
        if (!(err = ::pthread_timedjoin_relative_np(_thread, &value, &until_time))) {
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::pthread_timedjoin_relative_np(_thread, &value, &until_time)");
            is_forked = false;
            return join_success;
        } else {
            switch(err) {
            case EBUSY:
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...EBUSY err = " << err << " on ::pthread_timedjoin_relative_np(_thread, &value, &until_time)");
                return join_busy;
            case ETIMEDOUT:
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...ETIMEDOUT err = " << err << " on ::pthread_timedjoin_relative_np(_thread, &value, &until_time)");
                return join_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_timedjoin_relative_np(_thread, &value, &until_time)");
                is_forked = false;
                return join_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_timedjoin_relative_np(_thread, &value, &until_time)");
                is_forked = false;
            }
        }
#else /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
        LOGGER_IS_LOGGED_ERROR("...invalid ::pthread_timedjoin_relative_np(_thread, &value, &until_time)...");
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
        return join_failed; 
    }
    virtual join_status try_join_detached(bool& is_forked, thread_t& _thread) const { 
#if defined(PTHREAD_HAS_TRYJOIN_NP)
        void* value = 0;
        int err = 0;
        LOGGER_IS_LOGGED_TRACE("::pthread_tryjoin_np(_thread, &value)...");
        if (!(err = ::pthread_tryjoin_np(_thread, &value))) {
            LOGGER_IS_LOGGED_TRACE("...::pthread_tryjoin_np(_thread, &value)");
            is_forked = false;
            return join_success;
        } else {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_TRACE("...EBUSY err = " << err << " on ::pthread_tryjoin_np(_thread, &value)");
                return join_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_TRACE("...ETIMEDOUT err = " << err << " on ::pthread_tryjoin_np(_thread, &value)");
                return join_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_tryjoin_np(_thread, &value)");
                is_forked = false;
                return join_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_tryjoin_np(_thread, &value)");
                is_forked = false;
            }
        }
#else /// defined(PTHREAD_HAS_TRYJOIN_NP)
        LOGGER_IS_LOGGED_ERROR("...invalid ::pthread_tryjoin_np(_thread, &value)...");
#endif /// defined(PTHREAD_HAS_TRYJOIN_NP)
        return join_failed; 
    }
    virtual join_status untimed_join_detached(bool& is_forked, thread_t& _thread) const { 
        void* value = 0;
        int err = 0;
        LOGGER_IS_LOGGED_DEBUG("::pthread_join(_thread, &value)...");
        if (!(err = ::pthread_join(_thread, &value))) {
            LOGGER_IS_LOGGED_DEBUG("...::pthread_join(_thread, &value)");
            is_forked = false;
            return join_success;
        } else {
            switch(err) {
            case EBUSY:
                LOGGER_IS_LOGGED_ERROR("...EBUSY err = " << err << " on ::pthread_join(_thread, &value)");
                return join_busy;
            case ETIMEDOUT:
                LOGGER_IS_LOGGED_ERROR("...ETIMEDOUT err = " << err << " on ::pthread_join(_thread, &value)");
                return join_busy;
            case EINTR:
                LOGGER_IS_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_join(_thread, &value)");
                is_forked = false;
                return join_interrupted;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::pthread_join(_thread, &value)");
                is_forked = false;
            }
        }
        return join_failed; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_detached(thread_t& _thread) const {
        attached_t detached = (attached_t)(unattached);
        int err = EFAULT;
        pthread_attr_t threadattr;
        if (!(err = ::pthread_attr_init(&threadattr))) {
            if (!(err = ::pthread_create(&_thread, &threadattr, start_routine, ((void*)this)))) {
                if (!(err = ::pthread_attr_destroy(&threadattr))) {
                    detached = &_thread;
                } else {
                    bool is_forked = true;
                    untimed_join_detached(is_forked, _thread);
                }
                return detached;
            }
            if ((err = ::pthread_attr_destroy(&threadattr))) {
            }
        }
        return detached;
    }
    virtual bool destroy_detached(thread_t& _thread) const {
        return true;
    }

protected:
    /// run
    virtual pointer_t run(pointer_t parameter) {
        pointer_t result = 0;
        LOGGER_IS_LOGGED_DEBUG("...return result = " << pointer_to_string(result) << "...");
        return result;
    }

    /// start_routine
    static void* start_routine(void* param) {
        void* result = 0;
        threadt* t = 0;
        if ((t = ((threadt*)param))) {
            LOGGER_IF_LOGGED_DEBUG(t->is_logged(), "t->ran_.run()...");
            t->ran_.run();
            LOGGER_IF_LOGGED_DEBUG(t->is_logged(), "...t->ran_.run()");
        } else {
            LOGGER_LOG_ERROR("...unexpected 0 = ((threadt*)param)");
        }
        return result;
    }
}; /// class threadt
typedef threadt<> thread;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_POSIX_THREAD_HPP
