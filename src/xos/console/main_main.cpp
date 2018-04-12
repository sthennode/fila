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
///   File: main_main.cpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#include "xos/console/main_main.hpp"
#include "xos/console/main.hpp"
#include "xos/console/logger.hpp"
#include "xos/mt/os/mutex.hpp"

namespace xos {
namespace console {

} /// namespace console
} /// namespace xos

int main(int argc, char** argv, char** env) {
    int err = 0;
    ERR_LOG_DEBUG("try {...");
    try {
        ::xos::mt::os::mutex mutex(false);
        ::xos::console::std::derived::io io(mutex);
        ::xos::console::logger logger(io);

        LOG_DEBUG("::xos::console::main::the_main(argc, argv, env)...");
        err = ::xos::console::main::the_main(argc, argv, env);
        LOG_DEBUG("...err = " << err << " on ::xos::console::main::the_main(argc, argv, env)");

        LOG_DEBUG("...} try");
    } catch (const ::xos::exception& e) {
        ERR_LOG_ERROR("...catch (::xos::exception& e.status() = " << e.status_to_string() << ")");
        err = 1;
    } catch (...) {
        ERR_LOG_ERROR("...catch (...)");
        err = 1;
    }
    return err;
}
