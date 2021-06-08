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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 12/25/2019
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MUTEX_HPP
#define XOS_MT_MUTEX_HPP

#include "xos/platform/mutex.hpp"
#include "xos/base/time.hpp"
#include "xos/base/locked.hpp"
#include "xos/base/logged.hpp"
#include "xos/base/attached.hpp"
#include "xos/base/created.hpp"
#include "xos/base/logger.hpp"

namespace xos {
namespace mt {

/// class mutext
template 
<class TLogged = loggedt<locked>, 
 class TImplements = creatort<TLogged> >
class exported mutext: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef mutext derives;
}; /// class mutext
typedef mutext<> mutex;

namespace extended {
/// class mutext
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mt::mutex,  class TExtend = xos::extended::loggedt<TImplement>,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TExtends = TCreated, class TImplements = TAttacher>
class exported mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef mutext derives;

    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    /// constructor / destructor
    mutext(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
    }
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged) {
        this->set_is_logged(is_logged);
    }
    mutext(const mutext& copy): extends(copy) {
        this->set_is_logged(copy.is_logged_);
    }
    mutext() {
    }
    virtual ~mutext() {
        if (!(this->destroyed())) {
            throw (create_exception(destroy_failed));
        }
    }
}; /// class mutext
typedef mutext<> mutex;
} /// namespace extended

namespace derived {
/// class mutext
template 
<typename TMutex = platform_mutex_t,
 typename TAttr = platform_mutex_attr_t,
 TAttr VAttrNone = platform_mutex_attr_none,
 typename TError = platform_mutex_error_t,
 TError VErrorSuccess = platform_mutex_error_success,
 TError VErrorFailed = platform_mutex_error_failed,
 TError VErrorBusy = platform_mutex_error_busy,
 TError VErrorTimeout = platform_mutex_error_timeout,
 TError VErrorInterrupted = platform_mutex_error_interrupted,
 class TExtends = mt::extended::mutext<TMutex*>, 
 class TImplements = typename TExtends::implements>
class exported mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef mutext derives;

    typedef TMutex mutex_t;
    typedef TAttr mutex_attr_t;
    enum { mutex_attr_none = VAttrNone };
    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted};
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    mutext(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    mutext(const mutext& copy): extends(copy) {
    }
    mutext() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~mutext() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// ...lock... / ...unlock...
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_lock_detached(*detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_lock_detached(*detached);
        }
        return lock_failed;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_lock_detached(*detached);
        }
        return lock_failed;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return unlock_detached(*detached);
        }
        return false;
    }

    /// ...lock_detached... / ...unlock_detached...
    virtual bool lock_detached(mutex_t& mutex) const { 
        return (lock_success == (untimed_lock_detached(mutex))); 
    }
    virtual lock_status timed_lock_detached(mutex_t& mutex, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            error_t err = error_failed;
            platform_mutex_timeout until_time;

            until_time.tv_sec =  mseconds_seconds(milliseconds);
            until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::platform_mutex_timedlock(&mutex, &until_time)...");
            if (error_success == (err = ::platform_mutex_timedlock(&mutex, &until_time))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::platform_mutex_timedlock(&mutex, &until_time)");
                return lock_success;
            } else {
                if (error_busy == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...busy err = "<< err << " on ::platform_mutex_timedlock(&mutex, &until_time)");
                    return lock_busy;
                } else {
                    if (error_timeout == (err)) {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...timeout err = "<< err << " on ::platform_mutex_timedlock(&mutex, &until_time)");
                        return lock_busy;
                    } else {
                        if (error_interrupted == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_mutex_timedlock(&mutex, &until_time)");
                            return lock_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_mutex_timedlock(&mutex, &until_time)");
                        }
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_lock_detached(mutex);
            } else {
                return try_lock_detached(mutex);
            }
        }
        return lock_failed; 
    }
    virtual lock_status try_lock_detached(mutex_t& mutex) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_mutex_trylock(&mutex)...");
        if (error_success == (err = ::platform_mutex_trylock(&mutex))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_mutex_trylock(&mutex)");
            return lock_success;
        } else {
            if (error_busy == (err)) {
                LOGGER_IS_LOGGED_TRACE("...busy err = "<< err << " on ::platform_mutex_trylock(&mutex)");
                return lock_busy;
            } else {
                if (error_timeout == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...timeout err = "<< err << " on ::platform_mutex_trylock(&mutex)");
                    return lock_busy;
                } else {
                    if (error_interrupted == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_mutex_trylock(&mutex)");
                        return lock_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_mutex_trylock(&mutex)");
                    }
                }
            }
        }
        return lock_failed; 
    }
    virtual lock_status untimed_lock_detached(mutex_t& mutex) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_mutex_lock(&mutex)...");
        if (error_success == (err = ::platform_mutex_lock(&mutex))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_mutex_lock(&mutex)");
            return lock_success;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_mutex_lock(&mutex)");
        }
        return lock_failed; 
    }
    virtual bool unlock_detached(mutex_t& mutex) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_mutex_unlock(&mutex)...");
        if (error_success == (err = ::platform_mutex_unlock(&mutex))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_mutex_unlock(&mutex)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_mutex_unlock(&mutex)");
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(mutex_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached(mutex_t& mutex) const {
        attached_t detached = (attached_t)(unattached);
        mutex_attr_t mutex_attr = mutex_attr_none;
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_mutex_create(&mutex, mutex_attr)...");
        if (error_success == (err = ::platform_mutex_create(&mutex, mutex_attr))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_mutex_create(&mutex, mutex_attr)");
            detached = &mutex;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_mutex_create(&mutex, mutex_attr)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            error_t err = error_failed;
            LOGGER_IS_LOGGED_DEBUG("::platform_mutex_destroy(detached)...");
            if (error_success != ((err = ::platform_mutex_destroy(detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::platform_mutex_destroy(detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::platform_mutex_destroy(detached)");
                return true;
            }
        }
        return false;
    }

    /// wrapped
    virtual const mutex_t& wrapped() const {
        return mutex_;
    }

protected:
    mutex_t mutex_;
}; /// class mutext
typedef mutext<> mutex;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_MUTEX_HPP
