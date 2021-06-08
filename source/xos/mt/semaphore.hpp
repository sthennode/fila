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
///   File: semaphore.hpp
///
/// Author: $author$
///   Date: 1/5/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_SEMAPHORE_HPP
#define XOS_MT_SEMAPHORE_HPP

#include "xos/platform/semaphore.hpp"
#include "xos/base/acquired.hpp"
#include "xos/base/time.hpp"
#include "xos/base/logged.hpp"
#include "xos/base/attached.hpp"
#include "xos/base/created.hpp"
#include "xos/base/logger.hpp"

namespace xos {
namespace mt {

/// class semaphoret
template 
<class TLogged = loggedt<acquired>, 
 class TImplements = creatort<TLogged> >
class exported semaphoret: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef semaphoret derives;
}; /// class semaphoret
typedef semaphoret<> semaphore;

namespace extended {
/// class semaphoret
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mt::semaphore,  class TExtend = xos::extended::loggedt<TImplement>,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TExtends = TCreated, class TImplements = TAttacher>
class exported semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef semaphoret derives;

    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    /// constructor / destructor
    semaphoret(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
    }
    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged) {
        this->set_is_logged(is_logged);
    }
    semaphoret(const semaphoret& copy): extends(copy) {
        this->set_is_logged(copy.is_logged_);
    }
    semaphoret() {
    }
    virtual ~semaphoret() {
        if (!(this->destroyed())) {
            throw (create_exception(destroy_failed));
        }
    }

    /// create
    using extends::create;
    virtual bool create(size_t initially_released) {
        attached_t detached = (attached_t)(unattached);
        if (((attached_t)(unattached) != (detached = this->create_attached(initially_released)))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    using extends::create_attached;
    virtual attached_t create_attached(size_t initially_released) {
        attached_t detached = (attached_t)(unattached);
        if (!(initially_released)) {
            detached = create_attached();
        }
        return detached;
    }
}; /// class semaphoret
typedef semaphoret<> semaphore;
} /// namespace extended 

namespace derived {
/// class semaphoret
template 
<typename TWhat = platform_semaphore_t,
 typename TAttr = platform_semaphore_attr_t,
 TAttr VAttrNone = platform_semaphore_attr_none,
 typename TError = platform_semaphore_error_t,
 TError VErrorSuccess = platform_semaphore_error_success,
 TError VErrorFailed = platform_semaphore_error_failed,
 TError VErrorBusy = platform_semaphore_error_busy,
 TError VErrorTimeout = platform_semaphore_error_timeout,
 TError VErrorInterrupted = platform_semaphore_error_interrupted,
 class TExtends = mt::extended::semaphoret<TWhat*>, 
 class TImplements = typename TExtends::implements>
class exported semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef semaphoret derives;
    
    typedef TWhat semaphore_t;
    typedef TAttr semaphore_attr_t;
    enum { semaphore_attr_none = VAttrNone };
    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted };
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    semaphoret(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    semaphoret(const semaphoret& copy): extends(copy) {
    }
    semaphoret() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~semaphoret() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// ...acquire... / ...release...
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_acquire_detached(detached, milliseconds);
        }
        return acquire_failed;
    }
    virtual acquire_status untimed_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual acquire_status try_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual bool release() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return release_detached(detached);
        }
        return false;
    }

    /// ...acquire_detached... / ...release_detached...
    virtual bool acquire_detached(attached_t detached) const { 
        return (acquire_success == (untimed_acquire_detached(detached))); 
    }
    virtual acquire_status timed_acquire_detached(attached_t detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            if ((attached_t)(unattached) != (detached)) {
                bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
                error_t err = error_failed;
                platform_semaphore_timeout until_time;

                until_time.tv_sec =  mseconds_seconds(milliseconds);
                until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));

                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::platform_semaphore_timed_acquire(detached, &until_time)...");
                if (error_success == (err = ::platform_semaphore_timed_acquire(detached, &until_time))) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::platform_semaphore_timed_acquire(detached, &until_time)");
                    return acquire_success;
                } else {
                    if (error_busy == (err)) {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...error_busy = "<< err << " on ::platform_semaphore_timed_acquire(detached, &until_time)");
                        return acquire_busy;
                    } else {
                        if (error_timeout == (err)) {
                            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...error_timeout = "<< err << " on ::platform_semaphore_timed_acquire(detached, &until_time)");
                            return acquire_timeout;
                        } else {
                            if (error_interrupted == (err)) {
                                LOGGER_IS_LOGGED_ERROR("...error_interrupted = "<< err << " on ::platform_semaphore_timed_acquire(detached, &until_time)");
                                return acquire_interrupted;
                            } else {
                                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_semaphore_timed_acquire(detached, &until_time)");
                            }
                        }
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(detached);
            } else {
                return try_acquire_detached(detached);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            error_t err = error_failed;
            LOGGER_IS_LOGGED_TRACE("::platform_semaphore_try_acquire(detached)...");
            if (error_success == (err = ::platform_semaphore_try_acquire(detached))) {
                LOGGER_IS_LOGGED_TRACE("...::platform_semaphore_try_acquire(detached)");
                return acquire_success;
            } else {
                if (error_busy == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...error_busy = "<< err << " on ::platform_semaphore_try_acquire(detached)");
                    return acquire_busy;
                } else {
                    if (error_timeout == (err)) {
                        LOGGER_IS_LOGGED_TRACE("...error_timeout = "<< err << " on ::platform_semaphore_try_acquire(detached)");
                        return acquire_timeout;
                    } else {
                        if (error_interrupted == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...error_interrupted = "<< err << " on ::platform_semaphore_try_acquire(detached)");
                            return acquire_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_semaphore_try_acquire(detached)");
                        }
                    }
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            error_t err = error_failed;
            LOGGER_IS_LOGGED_DEBUG("::platform_semaphore_acquire(detached)...");
            if (error_success != ((err = ::platform_semaphore_acquire(detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::platform_semaphore_acquire(detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::platform_semaphore_acquire(detached)");
                return acquire_success;
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            error_t err = error_failed;
            LOGGER_IS_LOGGED_DEBUG("::platform_semaphore_release(detached)...");
            if (error_success != ((err = ::platform_semaphore_release(detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::platform_semaphore_release(detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::platform_semaphore_release(detached)");
                return true;
            }
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(semaphore_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached(semaphore_t& semaphore) const {
        attached_t detached = (attached_t)(unattached);
        semaphore_attr_t semaphore_attr = semaphore_attr_none;
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_semaphore_create(&semaphore, semaphore_attr)...");
        if (error_success == (err = ::platform_semaphore_create(&semaphore, semaphore_attr))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_semaphore_create(&semaphore, semaphore_attr)");
            detached = &semaphore;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_semaphore_create(&semaphore, semaphore_attr)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            error_t err = error_failed;
            LOGGER_IS_LOGGED_DEBUG("::platform_semaphore_destroy(detached)...");
            if (error_success != ((err = ::platform_semaphore_destroy(detached)))) {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::platform_semaphore_destroy(detached)");
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::platform_semaphore_destroy(detached)");
                return true;
            }
        }
        return false;
    }
    
protected:
    semaphore_t semaphore_;
}; /// class semaphoret
typedef semaphoret<> semaphore;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_SEMAPHORE_HPP 
