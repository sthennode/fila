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
///   File: os.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_OS_HPP
#define _XOS_MT_OS_OS_HPP

#include "xos/platform/platform_build.h"

namespace xos {
namespace mt {
namespace microsoft { namespace windows { namespace crt {} } }
namespace apple { namespace osx {} namespace ios {} namespace mach {} }
namespace linux {}
namespace posix {}
namespace os {

#if defined(WINDOWS)
namespace os = microsoft::windows;
#elif defined(MACOSX)
namespace os = apple::osx;
#elif defined(APPIOS)
namespace os = apple::ios;
#elif defined(LINUX)
namespace os = linux;
#else /// defined(WINDOWS)
namespace os = posix;
#endif /// defined(WINDOWS)

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_OS_HPP 
        

