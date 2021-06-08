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
///   File: threads.cpp
///
/// Author: $author$
///   Date: 1/14/2020
///////////////////////////////////////////////////////////////////////
#include "xos/mt/threads.hpp"
#include "xos/platform/os/platform/thread.cpp"

#if !defined(WINDOWS)
#include "xos/platform/os/microsoft/windows/Thread.cpp"
#endif /// !defined(WINDOWS)

/*/
#if !defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/thread.cpp"
#endif /// !defined(SOLARIS)

#if !defined(MACH) || defined(APPLE)
#include "xos/platform/os/mach/thread.cpp"
#endif /// !defined(MACH) || defined(APPLE)
/*/

namespace xos {
namespace mt {

} /// namespace mt
} /// namespace xos
