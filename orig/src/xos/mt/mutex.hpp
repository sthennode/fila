///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2018 $organization$
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
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MUTEX_HPP
#define _XOS_MT_MUTEX_HPP

#include "xos/base/logged.hpp"
#include "xos/base/locked.hpp"
#include "xos/base/created.hpp"
#include "xos/io/logger.hpp"

namespace xos {
namespace mt {

typedef loggedt<locked> mutext_implements;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = mutext_implements, 
 class TImplements = creatort<TImplement> >
class _EXPORT_CLASS mutext: virtual public TImplements {
public:
    typedef TImplements implements;
};
typedef mutext<> mutex;

namespace extended {

typedef mt::mutex mutext_implements;
typedef xos::extended::loggedt<mt::mutext_implements, base> mutext_extends;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mutext_implements,  class TExtend = mutext_extends,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TImplements = TAttacher, class TExtends = TCreated>
class _EXPORT_CLASS mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };
    
    mutext(attached_t detached, bool is_created)
    : extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged, bool is_err_logged) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_err_logged);
    }
    mutext(bool is_logged) {
        this->set_is_logged(is_logged);
    }
    mutext(const mutext &copy): extends(copy) {
        this->set_is_logged(copy.is_logged());
        this->set_is_err_logged(copy.is_err_logged());
    }
    mutext() {
    }
    virtual ~mutext() {
        IS_ERR_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            const create_exception e(destroy_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw (const create_exception e(destroy_failed))...");
            throw (e);
        }
    }

};
typedef mutext<> mutex;

} /// namespace extended

namespace derived {

typedef mt::mutex mutext_implements;
typedef mt::extended::mutex mutext_extends;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template <class TImplements = mutext_implements, class TExtends = mutext_extends>
class _EXPORT_CLASS mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };
    
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(bool is_logged): extends(is_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(const mutext &copy): extends(copy) {
    }
    mutext() {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    virtual ~mutext() {
        IS_ERR_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            const create_exception e(destroy_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw (const create_exception e(destroy_failed))...");
            throw (e);
        }
    }
};
typedef mutext<> mutex;

} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MUTEX_HPP 
