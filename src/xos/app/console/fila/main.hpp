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
///   File: main.hpp
///
/// Author: $author$
///   Date: 7/14/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_FILA_MAIN_HPP
#define _XOS_APP_CONSOLE_FILA_MAIN_HPP

#include "xos/mt/std/queue.hpp"
#include "xos/mt/os/semaphore.hpp"
#include "xos/mt/os/mutex.hpp"
#include "xos/console/getopt/main.hpp"

namespace xos {
namespace app {
namespace console {
namespace fila {

///////////////////////////////////////////////////////////////////////
///  Class: maint
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = xos::console::getopt::main::implements, 
 class TExtends = xos::console::getopt::main>
class _EXPORT_CLASS maint: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    maint() {
    }
    virtual ~maint() {
    }
private:
    maint(const maint &copy) {
    }
    
protected:
    virtual int run(int argc, char_t** argv, char_t** env) {
        mt::std::queuet<unsigned> queue_;
        return 0;
    }
};
typedef maint<> main;

} /// namespace fila
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_FILA_MAIN_HPP 
