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
#ifndef XOS_MT_CONDITION_HPP
#define XOS_MT_CONDITION_HPP

#include "xos/platform/condition.hpp"
#include "xos/mt/mutex.hpp"
#include "xos/base/time.hpp"
#include "xos/base/signaled.hpp"
#include "xos/base/logged.hpp"
#include "xos/base/attached.hpp"
#include "xos/base/created.hpp"
#include "xos/base/logger.hpp"

namespace xos {
namespace mt {

/// class conditiont
template 
<class TLogged = loggedt<signaled>, 
 class TImplements = creatort<TLogged> >
class exported conditiont: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef conditiont derives;
}; /// class conditiont
typedef conditiont<> condition;

namespace extended {
/// class conditiont
template 
<class TMutex = mutex, typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mt::condition,  class TExtend = xos::extended::loggedt<TImplement>,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TExtends = TCreated, class TImplements = TAttacher>
class exported conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef conditiont derives;
    
    typedef TMutex mutex_t;
    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    /// constructor / destructor
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged) {
        this->set_is_logged(is_logged);
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(attached_t detached): extends(detached) {
    }
    conditiont(bool is_logged) {
        this->set_is_logged(is_logged);
    }
    conditiont(const conditiont& copy): extends(copy) {
        this->set_is_logged(copy.is_logged_);
    }
    conditiont() {
    }
    virtual ~conditiont() {
        if (!(this->destroyed())) {
            throw (create_exception(destroy_failed));
        }
    }

}; /// class conditiont
typedef conditiont<> condition;
} /// namespace extended

namespace derive {
/// class conditiont
template 
<class TMutex = derived::mutex, 
 typename TCond = platform_condition_t,
 typename TError = platform_condition_error_t,
 TError VErrorSuccess = platform_condition_error_success,
 TError VErrorFailed = platform_condition_error_failed,
 TError VErrorBusy = platform_condition_error_busy,
 TError VErrorTimeout = platform_condition_error_timeout,
 TError VErrorInterrupted = platform_condition_error_interrupted,
 class TExtends = mt::extended::conditiont<TMutex, TCond*>, 
 class TImplements = typename TExtends::implements>
class exported conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef conditiont derives;
    
    typedef TMutex mutex_t;
    typedef TCond cond_t;

    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted };

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    typedef signal_status (derives::*wait_detached_t)(cond_t&, mutex_t&, mseconds_t) const;

    /// constructor / destructor
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(is_logged), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled): signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(0), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, attached_t detached, bool is_created): extends(detached, is_created), signaled_(0), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, attached_t detached): extends(detached), signaled_(0), mutex_(&mutex) {
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created), signaled_(0), mutex_(0) {
    }
    conditiont(attached_t detached): extends(detached), signaled_(0), mutex_(0) {
    }
    conditiont(bool is_logged): extends(is_logged), signaled_(0), mutex_(0) {
    }
    conditiont(const conditiont& copy): extends(copy) {
    }
    conditiont() {
    }
    virtual ~conditiont() {
        if (!(this->destroyed())) {
            throw (create_exception(destroy_failed));
        }
    }

    /// ...wait... / ...signal...
    virtual wait_status time_wait(mseconds_t milliseconds) {
        return try_timed_untimed_wait(&derives::time_wait_detached, milliseconds);
    }
    virtual wait_status timed_wait(mseconds_t milliseconds) {
        return try_timed_untimed_wait(&derives::timed_wait_detached, milliseconds);
        return wait_failed;
    }
    virtual wait_status untimed_wait() {
        return try_timed_untimed_wait(&derives::untimed_wait_detached, -1);
    }
    virtual wait_status try_wait() {
        return try_timed_untimed_wait(&derives::try_wait_detached, 0);
    }
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, mseconds_t milliseconds) { 
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            signal_status status = wait_failed;
            bool* signaled = 0;
            if (((signaled = signaled_))) {
                mutex_t* mutex = 0;
                if (((mutex = mutex_))) {
                    status = try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
            return status; 
        }
        return wait_failed;
    }
    virtual bool signal() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            bool success = false;
            bool* signaled = 0;
            if ((0 != (signaled = signaled_))) {
                mutex_t* mutex = 0;
                if ((0 != (mutex = mutex_))) {
                    success = signal_detached(*detached, *mutex, *signaled);
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
            return success;
        }
        return false;
    }

    /// ...wait_detached... / ...signal_detached...
    virtual wait_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, cond_t& cond, mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        if ((0 != (wait_detached))) {
            signal_status status = wait_failed;
            for (bool unsignaled = true; unsignaled; ) {
                try {
                    mutex_t locked(&mutex);
                    ::xos::lock lock(locked);
                    if (!(unsignaled = !signaled)) {
                        status = wait_success;
                    } else {
                        unsignaled = false;
                        if (wait_success == (status = (this->*wait_detached)(cond, mutex, milliseconds))) {
                            lock.set_unlocked();
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
            return status; 
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return wait_failed; 
    }
    virtual wait_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        signal_status status = wait_failed;
        if ((0 != (wait_detached))) {
            status = (this->*wait_detached)(cond, mutex, milliseconds);
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return status; 
    }
    virtual bool wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return (wait_success == (untimed_wait_detached(cond, mutex, milliseconds))); 
    }
    virtual wait_status time_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return wait_failed; 
    }
    virtual wait_status timed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return wait_failed; 
    }
    virtual wait_status try_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return wait_failed; 
    }
    virtual wait_status untimed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return wait_failed; 
    }
    virtual bool signal_detached(cond_t& cond, mutex_t& mutex, bool& signaled) const { 
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
    virtual bool signal_detached(cond_t& cond) const { 
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(cond_, mutex_)))) {
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
    virtual attached_t create_detached(cond_t& cond, mutex_t* mutex) const {
        attached_t detached = (attached_t)(unattached);
        if ((mutex)) {
            detached = create_detached(cond, *mutex);
        }
        return detached;
    }
    virtual attached_t create_detached(cond_t& cond, mutex_t& mutex) const {
        attached_t detached = (attached_t)(unattached);
        detached = create_detached(cond);
        return detached;
    }
    virtual attached_t create_detached(cond_t& cond) const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual bool destroy_detached(cond_t& cond) const {
        return false;
    }

    /// wrapped
    virtual const cond_t& wrapped() const {
        return cond_;
    }

protected:
    bool* signaled_;
    mutex_t* mutex_;
    cond_t cond_;
}; /// class conditiont
typedef conditiont<> condition;
} /// namespace derive

namespace derived {
/// class conditiont
template 
<class TMutex = derived::mutex, 
 typename TCond = platform_condition_t,
 typename TCondAttr = platform_condition_attr_t,
 typename TCondAttrNoneT = platform_condition_attr_none_t,
 TCondAttrNoneT VCondAttrNone = platform_condition_attr_none,
 typename TError = platform_condition_error_t,
 TError VErrorSuccess = platform_condition_error_success,
 TError VErrorFailed = platform_condition_error_failed,
 TError VErrorBusy = platform_condition_error_busy,
 TError VErrorTimeout = platform_condition_error_timeout,
 TError VErrorInterrupted = platform_condition_error_interrupted,
 class TExtends = mt::derive::conditiont
 <TMutex, TCond, TError, VErrorSuccess, VErrorFailed, VErrorBusy, VErrorTimeout, VErrorInterrupted>, 
 class TImplements = typename TExtends::implements>
class exported conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef conditiont derives;
    
    typedef TMutex mutex_t;
    typedef TCond cond_t;
    typedef TCondAttr cond_attr_t;
    typedef TCondAttrNoneT cond_attr_none_t;
    enum { cond_attr_none = VCondAttrNone };
    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted};
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /*/
    typedef signal_status (derives::*wait_detached_t)(cond_t&, mutex_t&, mseconds_t) const;

    /// constructor / destructor
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached), signaled_(&signaled), mutex_(&mutex) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(is_logged), signaled_(&signaled), mutex_(&mutex) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    conditiont(mutex_t& mutex, bool& signaled): signaled_(&signaled), mutex_(&mutex) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
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
        return wait_failed;
    }
    virtual wait_status untimed_wait() {
        return try_timed_untimed_wait(&derives::untimed_wait_detached, -1);
    }
    virtual wait_status try_wait() {
        return try_timed_untimed_wait(&derives::try_wait_detached, 0);
    }
    virtual signal_status try_timed_untimed_wait(wait_detached_t wait_detached, mseconds_t milliseconds) { 
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            signal_status status = wait_failed;
            bool* signaled = 0;
            if (((signaled = signaled_))) {
                mutex_t* mutex = 0;
                if (((mutex = mutex_))) {
                    status = try_timed_untimed_wait_detached(wait_detached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
            return status; 
        }
        return wait_failed;
    }
    virtual bool signal() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            bool success = false;
            bool* signaled = 0;
            if ((0 != (signaled = signaled_))) {
                mutex_t* mutex = 0;
                if ((0 != (mutex = mutex_))) {
                    success = signal_detached(*detached, *mutex, *signaled);
                } else {
                    LOGGER_IS_LOGGED_ERROR("...failed 0 = (mutex_)");
                }
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed 0 = (signaled_)");
            }
            return success;
        }
        return false;
    }

    /// ...wait_detached... / ...signal_detached...
    virtual wait_status try_timed_untimed_wait_detached(wait_detached_t wait_detached, cond_t& cond, mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        if ((0 != (wait_detached))) {
            signal_status status = wait_failed;
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
            return status; 
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed 0 = (wait_Detached)");
        }
        return wait_failed; 
    }
    virtual bool wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        return (wait_success == (untimed_wait_detached(cond, mutex, milliseconds))); 
    }
/*/
    virtual wait_status time_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
        error_t err = error_failed;
        platform_condition_timeout until_time;
        until_time.tv_sec =  mseconds_seconds(milliseconds);
        until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::platform_condition_time_wait(&cond, &until_time)...");
        if (error_success == (err = ::platform_condition_time_wait(&cond, &until_time))) {
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::platform_condition_time_wait(&cond, &until_time)");
            return wait_success;
        } else {
            if (error_busy == (err)) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...busy err = "<< err << " on ::platform_condition_time_wait(&cond, &until_time)");
                return wait_busy;
            } else {
                if (error_timeout == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...timeout err = "<< err << " on ::platform_condition_time_wait(&cond, &until_time)");
                    return wait_busy;
                } else {
                    if (error_interrupted == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_condition_time_wait(&cond, &until_time)");
                        return wait_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_time_wait(&cond, &until_time)");
                    }
                }
            }
        }
        return wait_failed; 
    }
    virtual wait_status timed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            bool is_logged = ((this->is_logged()) && (milliseconds >= this->is_logged_debug_threashold()));
            error_t err = error_failed;
            platform_condition_timeout until_time;
            until_time.tv_sec =  mseconds_seconds(milliseconds);
            until_time.tv_nsec =  mseconds_nseconds(mseconds_mseconds(milliseconds));
            LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "::platform_condition_timed_wait(&cond, &until_time)...");
            if (error_success == (err = ::platform_condition_timed_wait(&cond, &until_time))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...::platform_condition_timed_wait(&cond, &until_time)");
                return wait_success;
            } else {
                if (error_busy == (err)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...busy err = "<< err << " on ::platform_condition_timed_wait(&cond, &until_time)");
                    return wait_busy;
                } else {
                    if (error_timeout == (err)) {
                        LOGGER_IF_LOGGED_DEBUG_TRACE(is_logged, is_logged, "...timeout err = "<< err << " on ::platform_condition_timed_wait(&cond, &until_time)");
                        return wait_busy;
                    } else {
                        if (error_interrupted == (err)) {
                            LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_condition_timed_wait(&cond, &until_time)");
                            return wait_interrupted;
                        } else {
                            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_timed_wait(&cond, &until_time)");
                        }
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_wait_detached(cond, mutex, milliseconds);
            } else {
                return try_wait_detached(cond, mutex, milliseconds);
            }
        }
        return wait_failed; 
    }
    virtual wait_status try_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_TRACE("::platform_condition_try_wait(&cond)...");
        if (error_success == (err = ::platform_condition_try_wait(&cond))) {
            LOGGER_IS_LOGGED_TRACE("...::platform_condition_try_wait(&cond)");
            return wait_success;
        } else {
            if (error_busy == (err)) {
                LOGGER_IS_LOGGED_TRACE("...busy err = "<< err << " on ::platform_condition_try_wait(&cond)");
                return wait_busy;
            } else {
                if (error_timeout == (err)) {
                    LOGGER_IS_LOGGED_TRACE("...timeout err = "<< err << " on ::platform_condition_try_wait(&cond)");
                    return wait_busy;
                } else {
                    if (error_interrupted == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_condition_try_wait(&cond)");
                        return wait_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_try_wait(&cond)");
                    }
                }
            }
        }
        return wait_failed; 
    }
    virtual wait_status untimed_wait_detached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_condition_wait(&cond)...");
        if (error_success == (err = ::platform_condition_wait(&cond))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_condition_wait(&cond)");
            return wait_success;
        } else {
            if (error_busy == (err)) {
                LOGGER_IS_LOGGED_DEBUG("...busy err = "<< err << " on ::platform_condition_wait(&cond)");
                return wait_busy;
            } else {
                if (error_timeout == (err)) {
                    LOGGER_IS_LOGGED_DEBUG("...timeout err = "<< err << " on ::platform_condition_wait(&cond)");
                    return wait_busy;
                } else {
                    if (error_interrupted == (err)) {
                        LOGGER_IS_LOGGED_ERROR("...interrupted err = "<< err << " on ::platform_condition_wait(&cond)");
                        return wait_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_wait(&cond)");
                    }
                }
            }
        }
        return wait_failed; 
    }
/*/
    virtual bool signal_detached(cond_t& cond, mutex_t& mutex, bool& signaled) const { 
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
    virtual bool signal_detached(cond_t& cond) const { 
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_condition_signal(&cond)...");
        if (error_success == (err = ::platform_condition_signal(&cond))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_condition_signal(&cond)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_signal(&cond)");
        }
        return false; 
    }
/*/
    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(cond_, mutex_)))) {
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
    virtual attached_t create_detached(cond_t& cond, mutex_t* mutex) const {
        attached_t detached = (attached_t)(unattached);
        if ((mutex)) {
            cond_attr_t cond_attr = ((cond_attr_t)mutex->wrapped());
            error_t err = error_failed;
            LOGGER_IS_LOGGED_DEBUG("::platform_condition_create(&cond, cond_attr)...");
            if (error_success == (err = ::platform_condition_create(&cond, cond_attr))) {
                LOGGER_IS_LOGGED_DEBUG("...::platform_condition_create(&cond, cond_attr)");
                detached = &cond;
            } else {
                LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_create(&cond, cond_attr)");
            }
        }
        return detached;
    }
/*/
    /// ...create... / ...destroy...
    virtual attached_t create_detached(cond_t& cond, mutex_t& mutex) const {
        attached_t detached = (attached_t)(unattached);
        cond_attr_t cond_attr = ((cond_attr_t)mutex.wrapped());
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_condition_create(&cond, cond_attr)...");
        if (error_success == (err = ::platform_condition_create(&cond, cond_attr))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_condition_create(&cond, cond_attr)");
            detached = &cond;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " on ::platform_condition_create(&cond, cond_attr)");
        }
        return detached;
    }
    virtual bool destroy_detached(cond_t& cond) const {
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_condition_destroy(&cond)...");
        if (error_success != ((err = ::platform_condition_destroy(&cond)))) {
            LOGGER_IS_LOGGED_ERROR("...failed err = "<< err << " ::platform_condition_destroy(&cond)");
        } else {
            LOGGER_IS_LOGGED_DEBUG("...::platform_condition_destroy(&cond)");
            return true;
        }
        return false;
    }
/*/
    /// wrapped
    virtual const cond_t& wrapped() const {
        return cond_;
    }

protected:
    bool* signaled_;
    mutex_t* mutex_;
    cond_t cond_;
/*/
}; /// class conditiont
typedef conditiont<> condition;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_CONDITION_HPP
