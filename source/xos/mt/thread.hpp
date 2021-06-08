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
///   Date: 1/20/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_THREAD_HPP
#define XOS_MT_THREAD_HPP

#include "xos/platform/thread.hpp"
#include "xos/base/time.hpp"
#include "xos/base/suspended.hpp"
#include "xos/base/joined.hpp"
#include "xos/base/ran.hpp"
#include "xos/base/logged.hpp"
#include "xos/base/attached.hpp"
#include "xos/base/created.hpp"
#include "xos/base/logger.hpp"

namespace xos {
namespace mt {

/// class thread_joinedt
template <class TRan = ran, class TSuspended = suspended, class TJoined = joined>
class exported thread_joinedt: virtual public TRan, virtual public TSuspended, virtual public TJoined {
public:
    typedef TRan ran_t;
    typedef TJoined joined_t;
    typedef TSuspended suspended_t;
    typedef thread_joinedt derives;
}; /// class thread_joinedt
typedef thread_joinedt<> thread_joined;

/// class threadt
template 
<class TLogged = loggedt<thread_joined>, 
 class TImplements = creatort<TLogged> >
class exported threadt: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef threadt derives;

}; /// class threadt
typedef threadt<> thread;

namespace extended {
/// class threadt
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mt::thread,  class TExtend = xos::extended::loggedt<TImplement>,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TExtends = TCreated, class TImplements = TAttacher>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef typename implements::ran_t ran_t;
    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    /// constructor / destructor
    threadt(ran_t& ran, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created), ran_(ran), is_forked_(is_created) {
        this->set_is_logged(is_logged);
    }
    threadt(ran_t& ran, attached_t detached, bool is_created): extends(detached, is_created), ran_(ran), is_forked_(is_created) {
    }
    threadt(ran_t& ran, attached_t detached): extends(detached), ran_(ran), is_forked_(false) {
    }
    threadt(ran_t& ran, bool is_logged): ran_(ran), is_forked_(false) {
        this->set_is_logged(is_logged);
    }
    threadt(ran_t& ran): ran_(ran), is_forked_(false) {
    }
    /// constructor / destructor
    threadt(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created), ran_(*this), is_forked_(is_created) {
        this->set_is_logged(is_logged);
    }
    threadt(attached_t detached, bool is_created): extends(detached, is_created), ran_(*this), is_forked_(is_created) {
    }
    threadt(attached_t detached): extends(detached), ran_(*this), is_forked_(false) {
    }
    threadt(bool is_logged): ran_(*this) {
        this->set_is_logged(is_logged);
    }
    threadt(const threadt& copy): extends(copy), ran_(*this), is_forked_(false) {
        this->set_is_logged(copy.is_logged_);
    }
    threadt(): ran_(*this), is_forked_(false) {
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            throw join_exception(join_failed);
        }
        if (!(this->destroyed())) {
            throw create_exception(destroy_failed);
        }
    }

    /// create / destroy / detach
    virtual bool create(bool initially_suspended) {
        if (!(initially_suspended)) {
            return create();
        }
        return false;
    }
    virtual bool create() {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = this->create_attached())) {
            this->set_is_created();
            this->set_is_forked();
            return true;
        }
        return false;
    }
    virtual bool destroy() {
        if ((this->joined())) {
            attached_t detached = (attached_t)(unattached);
            if ((attached_t)(unattached) != (detached = this->detach())) {
                if ((this->destroy_detached(detached))) {
                    return true;
                }
            }
        }
        return false;
    }
    virtual attached_t detach() {
        attached_t detached = extends::detach();
        this->set_is_forked(false);
        return detached;
    }

    /// ...is_joined
    virtual bool set_is_joined(bool to = true) {
        is_forked_ = !to;
        return !is_forked_;
    }
    virtual bool is_joined() const {
        return !is_forked_;
    }

    /// ...is_forked
    virtual bool set_is_forked(bool to = true) {
        is_forked_ = to;
        return is_forked_;
    }
    virtual bool is_forked() const {
        return is_forked_;
    }

protected:
    ran_t& ran_;
    bool is_forked_;
}; /// class threadt
typedef threadt<> thread;
} /// namespace extended

namespace derive {
/// class threadt
template 
<typename TThread = platform_thread_t,
 typename TError = platform_thread_error_t,
 TError VErrorSuccess = platform_thread_error_success,
 TError VErrorFailed = platform_thread_error_failed,
 TError VErrorBusy = platform_thread_error_busy,
 TError VErrorTimeout = platform_thread_error_timeout,
 TError VErrorInterrupted = platform_thread_error_interrupted,
 typename TAttached = TThread*,
 class TExtends = mt::extended::threadt<TAttached>, 
 class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef TThread thread_t;

    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted };

    typedef typename extends::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    threadt(ran_t& ran, attached_t detached, bool is_created, bool is_logged): extends(ran, detached, is_created, is_logged) {
    }
    threadt(ran_t& ran, attached_t detached, bool is_created): extends(ran, detached, is_created) {
    }
    threadt(ran_t& ran, attached_t detached): extends(ran, detached) {
    }
    threadt(ran_t& ran, bool is_logged): extends(ran, is_logged) {
    }
    threadt(ran_t& ran): extends(ran) {
    }
    /// constructor / destructor
    threadt(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    threadt(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    threadt(attached_t detached): extends(detached) {
    }
    threadt(bool is_logged): extends(is_logged) {
    }
    threadt(const threadt& copy): extends(copy) {
    }
    threadt() {
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            throw join_exception(join_failed);
        }
    }

    /// ...join... / ...fork...
    virtual join_status time_join(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return time_join_detached(this->is_forked_, *detached, milliseconds);
        }
        return join_failed;
    }
    virtual join_status timed_join(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_join_detached(this->is_forked_, *detached, milliseconds);
        }
        return join_failed;
    }
    virtual join_status untimed_join() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_join_detached(this->is_forked_, *detached);
        }
        return join_failed;
    }
    virtual join_status try_join() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_join_detached(this->is_forked_, *detached);
        }
        return join_failed;
    }

    /// ...join_detached... / ...fork_detached...
    virtual bool join_detached(bool& is_forked, thread_t& _thread) const { 
        return (join_success == (untimed_join_detached(is_forked, _thread))); 
    }
    virtual join_status time_join_detached(bool& is_forked, thread_t& _thread, mseconds_t milliseconds) const { 
        return join_failed; 
    }
    virtual join_status timed_join_detached(bool& is_forked, thread_t& _thread, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
        } else {
            if (0 > (milliseconds)) {
                return untimed_join_detached(is_forked, _thread);
            } else {
                return try_join_detached(is_forked, _thread);
            }
        }
        return join_failed; 
    }
    virtual join_status try_join_detached(bool& is_forked, thread_t& _thread) const { 
        return join_failed; 
    }
    virtual join_status untimed_join_detached(bool& is_forked, thread_t& _thread) const { 
        return join_failed; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(thread_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            return destroy_detached(*detached);
        }
        return false;
    }
    virtual attached_t create_detached(thread_t& _thread) const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual bool destroy_detached(thread_t& _thread) const {
        return false;
    }
    
protected:
    thread_t thread_;
}; /// class threadt
typedef threadt<> thread;
} /// namespace derive

namespace derived {
/// class threadt
template 
<typename TThread = platform_thread_t,
 typename TThreadAttr = platform_thread_attr_t,
 typename TThreadAttrNone = platform_thread_attr_none_t,
 TThreadAttrNone VthreadAttrNone = platform_thread_attr_none,
 typename TError = platform_thread_error_t,
 TError VErrorSuccess = platform_thread_error_success,
 TError VErrorFailed = platform_thread_error_failed,
 TError VErrorBusy = platform_thread_error_busy,
 TError VErrorTimeout = platform_thread_error_timeout,
 TError VErrorInterrupted = platform_thread_error_interrupted,
 class TExtends = mt::derive::threadt
 <TThread, TError, VErrorSuccess, VErrorFailed, VErrorBusy, VErrorTimeout, VErrorInterrupted>, 
 class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef TThread thread_t;
    typedef TThreadAttr thread_attr_t;
    typedef TThreadAttrNone thread_attr_none_t;
    enum { thread_attr_none = VthreadAttrNone };

    typedef TError error_t;
    enum { error_success = VErrorSuccess,
           error_failed = VErrorFailed,
           error_busy = VErrorBusy,
           error_timeout = VErrorTimeout,
           error_interrupted = VErrorInterrupted};

    typedef typename extends::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    threadt(ran_t& ran, attached_t detached, bool is_created, bool is_logged): extends(ran, detached, is_created, is_logged) {
    }
    threadt(ran_t& ran, attached_t detached, bool is_created): extends(ran, detached, is_created) {
    }
    threadt(ran_t& ran, attached_t detached): extends(ran, detached) {
    }
    threadt(ran_t& ran, bool is_logged): extends(ran, is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(ran_t& ran): extends(ran) {
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
        return join_failed; 
    }
    virtual join_status timed_join_detached(bool& is_forked, thread_t& _thread, mseconds_t milliseconds) const { 
        return join_failed; 
    }
    virtual join_status try_join_detached(bool& is_forked, thread_t& _thread) const { 
        return join_failed; 
    }
    virtual join_status untimed_join_detached(bool& is_forked, thread_t& _thread) const { 
        ::platform_thread_error_t err = ::platform_thread_error_failed;
        if (::platform_thread_error_success == (err = ::platform_thread_join(&_thread))) {
            is_forked = false;
            return join_success;
        } else {
            switch (err) {
            case ::platform_thread_error_busy:
                return join_busy;
            case ::platform_thread_error_timeout:
                return join_timeout;
            case ::platform_thread_error_interrupted:
                is_forked = false;
                return join_interrupted;
            }
            is_forked = false;
        }
        return join_failed; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_detached(thread_t& _thread) const {
        attached_t detached = (attached_t)(unattached);
        thread_attr_t thread_attr = ((thread_attr_t)(&this->ran_));
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_thread_create(&_thread, thread_attr, start_routine)...");
        if (!(err = ::platform_thread_create(&_thread, thread_attr, start_routine))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_thread_create(&_thread, thread_attr, start_routine)");
            detached = &_thread;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::platform_thread_create(&_thread, thread_attr, start_routine)");
        }
        return detached;
    }
    virtual bool destroy_detached(thread_t& _thread) const {
        error_t err = error_failed;
        LOGGER_IS_LOGGED_DEBUG("::platform_thread_destroy(&_thread)...");
        if (!(err = ::platform_thread_destroy(&_thread))) {
            LOGGER_IS_LOGGED_DEBUG("...::platform_thread_destroy(&_thread)");
            return true;
        } else {
            LOGGER_IS_LOGGED_ERROR("...failed err = " << err << "on ::platform_thread_destroy(&_thread)");
        }
        return false;
    }

protected:
    /// start_routine
    static pointer_t start_routine(pointer_t parameter) {
        pointer_t result = 0;
        ran_t* ran = 0;
        if ((ran = ((ran_t*)parameter))) {
            result = ran->run();
        }
        return result;
    }
}; /// class threadt
typedef threadt<> thread;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_THREAD_HPP
