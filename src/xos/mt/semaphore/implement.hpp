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
//   File: implement.hpp
//
// Author: $author$
//   Date: 8/16/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_SEMAPHORE_IMPLEMENT_HPP
#define _XOS_MT_SEMAPHORE_IMPLEMENT_HPP

#include "xos/mt/semaphore/interface.hpp"

namespace xos {
namespace mt {
namespace semaphore {

///////////////////////////////////////////////////////////////////////
//  Class: implementt
///////////////////////////////////////////////////////////////////////
template <class Timplements = semaphore::interface>

class _EXPORT_CLASS implementt: virtual public Timplements {
public:
    typedef Timplements implements;
}; // class _EXPORT_CLASS implementt

typedef implementt<> implement;

} // namespace semaphore
} // namespace mt
} // namespace xos

#endif // _XOS_MT_SEMAPHORE_IMPLEMENT_HPP 
