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
///   File: version.hpp
///
/// Author: $author$
///   Date: 12/18/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_LIB_FILA_VERSION_HPP
#define XOS_LIB_FILA_VERSION_HPP

#include "xos/lib/version.hpp"

namespace xos {
namespace lib {
namespace fila {

/// class version
class exported version {
public:
    /// which
    static const lib::version& which();
}; /// class version

} /// namespace fila
} /// namespace lib
} /// namespace xos

#endif /// ndef XOS_LIB_FILA_VERSION_HPP 
