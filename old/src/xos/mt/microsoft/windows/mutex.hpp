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
#ifndef _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP
#define _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP

#include "xos/base/mutex.hpp"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

typedef mutex mutext_implements;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template <class TImplements = mutext_implements>
class _EXPORT_CLASS mutext: virtual public TImplements {
public:
    typedef TImplements implements;
};
typedef mutext<> mutex;

typedef mutex mutext_implements;
typedef mutex_extend mutext_extends;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template <class TImplements = mutext_implements, class TExtends = mutext_extends>
class _EXPORT_CLASS mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    mutext(const mutext &copy) {
    }
    mutext() {
    }
    virtual ~mutext() {
    }
};
typedef mutext<> mutex;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP 
        

