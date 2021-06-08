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
///   File: mutex.cpp
///
/// Author: $author$
///   Date: 12/26/2019
///////////////////////////////////////////////////////////////////////
#include "xos/mt/os/mutex.hpp"

#if defined(WINDOWS)  
#include "xos/mt/microsoft/windows/mutex.cpp"
#elif defined(APPLEOSX)  
#include "xos/mt/apple/osx/mutex.cpp"
#elif defined(APPLEIOS)  
#include "xos/mt/apple/ios/mutex.cpp"
#elif defined(SOLARIS)  
#include "xos/mt/oracle/solaris/mutex.cpp"
#elif defined(MACH)  
#include "xos/mt/mach/mutex.cpp"
#elif defined(LINUX)  
#include "xos/mt/linux/mutex.cpp"
#else /// defined(LINUX)  
#include "xos/mt/posix/mutex.cpp"
#endif /// defined(LINUX) 

namespace xos {
namespace mt {
namespace os {

} /// namespace os
} /// namespace mt
} /// namespace xos
