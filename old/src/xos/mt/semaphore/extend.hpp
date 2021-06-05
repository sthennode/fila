///////////////////////////////////////////////////////////////////////
// Copyright (c) 1988-2018 $organization$
//
// This software is provided by the author and contributors ``as is'' 
// and any express or implied warranties, including, but not limited to, 
// the implied warranties of merchantability and fitness for a particular 
// purpose are disclaimed. In no event shall the author or contributors 
// be liable for any direct, indirect, incidental, special, exemplary, 
// or consequential damages (including, but not limited to, procurement 
// of substitute goods or services; loss of use, data, or profits; or 
// business interruption) however caused and on any theory of liability, 
// whether in contract, strict liability, or tort (including negligence 
// or otherwise) arising in any way out of the use of this software, 
// even if advised of the possibility of such damage.
//
//   File: extend.hpp
//
// Author: $author$
//   Date: 8/16/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_SEMAPHORE_EXTEND_HPP
#define _XOS_MT_SEMAPHORE_EXTEND_HPP

#include "xos/mt/semaphore/implement.hpp"

namespace xos {
namespace mt {
namespace semaphore {

///////////////////////////////////////////////////////////////////////
//  Class: extendt
///////////////////////////////////////////////////////////////////////
template 
<typename Tattached = pointer_t, 
 typename Tunattached = int, Tunattached Vunattached = 0,
 class Textends = createdt
 <Tattached, Tunattached, Vunattached, 
  semaphore::implement, logged_extendt<semaphore::implement> > >

class _EXPORT_CLASS extendt: public Textends {
public:
    typedef Textends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    extendt(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    extendt(attached_t detached): extends(detached) {
    }
    extendt(const extendt& copy): extends(copy) {
    }
    extendt(bool is_logged, bool is_err_logged) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_err_logged);
    }
    extendt(bool is_logged) {
        this->set_is_logged(is_logged);
    }
    extendt() {
    }
    virtual ~extendt() {
    }
}; // class _EXPORT_CLASS extendt

typedef extendt<> extend;

///////////////////////////////////////////////////////////////////////
//  Class: releasedt
///////////////////////////////////////////////////////////////////////
template <class Textends = semaphore::extend>
class _EXPORT_CLASS releasedt: public Textends {
public:
    typedef Textends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    releasedt(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    releasedt(attached_t detached): extends(detached) {
    }
    releasedt(const releasedt& copy): extends(copy) {
    }
    releasedt(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
    }
    releasedt(bool is_logged): extends(is_logged) {
    }
    releasedt() {
    }
    virtual ~releasedt() {
    }

    virtual bool acquire() { 
        return true; 
    }
    virtual bool realease() { 
        return true; 
    }
    virtual acquire_status try_acquire() { 
        return acquire_success; 
    }
    virtual acquire_status untimed_acquire() { 
        return acquire_success; 
    }
    virtual acquire_status timed_acquire(mseconds_t milliseconds) { 
        return acquire_success; 
    }
}; // class _EXPORT_CLASS releasedt

typedef releasedt<> released;

} // namespace semaphore
} // namespace mt
} // namespace xos

#endif // _XOS_MT_SEMAPHORE_EXTEND_HPP 
