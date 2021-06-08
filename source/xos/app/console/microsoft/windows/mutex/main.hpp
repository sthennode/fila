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
///   File: main.hpp
///
/// Author: $author$
///   Date: 1/29/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_APP_CONSOLE_MICROSOFT_WINDOWS_MUTEX_MAIN_HPP
#define XOS_APP_CONSOLE_MICROSOFT_WINDOWS_MUTEX_MAIN_HPP

#include "xos/app/console/microsoft/windows/mutex/main_opt.hpp"
#include "xos/mt/microsoft/windows/mutex.hpp"

namespace xos {
namespace app {
namespace console {
namespace microsoft {
namespace windows {
namespace mutex {

/// class maint
template 
<class TExtends = main_opt, 
 class TImplements = typename TExtends::implements>

class exported maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef maint derives;

    typedef typename extends::string_t string_t;
    typedef typename extends::char_t char_t;
    typedef typename extends::end_char_t end_char_t;
    enum { end_char = extends::end_char };

    /// constructor / destructor
    maint() {
    }
    virtual ~maint() {
    }
private:
    maint(const maint& copy) {
    }

protected:
    /// ...run
    virtual int windows_run(int argc, char_t** argv, char_t** env) {
        return this->template run< ::xos::mt::microsoft::windows::mutex >();
    }
    virtual int os_run(int argc, char_t** argv, char_t** env) {
        return windows_run(argc, argv, env);
    }
}; /// class maint
typedef maint<> main;

} /// namespace mutex
} /// namespace windows
} /// namespace microsoft
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// XOS_APP_CONSOLE_MICROSOFT_WINDOWS_MUTEX_MAIN_HPP
