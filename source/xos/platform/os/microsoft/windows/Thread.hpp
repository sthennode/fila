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
///   File: Thread.hpp
///
/// Author: $author$
///   Date: 1/23/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_PLATFORM_OS_MICROSOFT_WINDOWS_THREAD_HPP
#define XOS_PLATFORM_OS_MICROSOFT_WINDOWS_THREAD_HPP

#include "xos/mt/os/thread.hpp"
#include "xos/platform/os/microsoft/windows/Handle.hpp"
#include "xos/platform/os/microsoft/windows/Thread.h"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

namespace internal {
/// class threadt
template <typename TStartAddress, typename TParameter, typename TResult, class TExtends = mt::os::thread>
class exported threadt: public TExtends {
public:
    typedef TExtends extends;
    typedef threadt derives; 
    
    typedef TStartAddress StartAddress;
    typedef TParameter Parameter;
    typedef TResult Result;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };
    
    /// constructors / destructor
    threadt(StartAddress startAddress, Parameter parameter)
    : extends(((attached_t)unattached), false, false), 
      _startAddress(startAddress), _parameter(parameter) {
    }
    virtual ~threadt() {
    }
private:
    threadt(const threadt& copy) {
        throw (exception(exception_unexpected));
    }

protected:
    /// run
    virtual pointer_t run(pointer_t parameter) {
        pointer_t result = 0;
        if ((_startAddress)) {
            Result _result = (*_startAddress)(_parameter);
        }
        return result;
    }

protected:
    StartAddress _startAddress;
    Parameter _parameter;
}; /// class threadt
typedef DWORD (WINAPI *StartAddress)(LPVOID);
typedef threadt<StartAddress, LPVOID, DWORD> thread;
} /// namespace internal

/// class threadt
template <class TThread, class TExtends = Handle, class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives; 
    
    typedef TThread Thread;
    typedef typename Thread::StartAddress StartAddress;
    typedef typename Thread::Parameter Parameter;

    /// constructors / destructor
    threadt(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded): _thread(startAddress, parameter) {
        if (!(_thread.create(initiallySuspeneded))) {
            throw create_exception(create_failed);
        }
    }
    threadt(StartAddress startAddress, Parameter parameter): _thread(startAddress, parameter) {
        if (!(_thread.create())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~threadt() {
    }
private:
    threadt(const threadt& copy) {
        throw (exception(exception_unexpected));
    }

public:
    /// CloseHandle
    virtual BOOL CloseHandle() {
        BOOL success = (_thread.destroy())?(TRUE):(FALSE);
        return success;
    }
    
    /// WaitForSingleObject
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::join_status status = ::xos::join_failed;

        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = _thread.timed_join(dwMilliseconds);
            } else {
                status = _thread.try_join();
            }
        } else {
            status = _thread.untimed_join();
        }
        switch (status) {
        case ::xos::join_success:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::join_busy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::join_interrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }

protected:
    Thread _thread;
}; /// class threadt

/// class threadtt
template <class TExtends = threadt<internal::thread>, class TImplements = typename TExtends::implements>
class exported threadtt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadtt derives; 
    
    typedef typename extends::StartAddress StartAddress;
    typedef typename extends::Parameter Parameter;

    /// constructors / destructor
    threadtt(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded): extends(startAddress, parameter, initiallySuspeneded) {
    }
    threadtt(StartAddress startAddress, Parameter parameter): extends(startAddress, parameter) {
    }
    virtual ~threadtt() {
    }
private:
    threadtt(const threadtt& copy): extends(0, 0) {
        throw (exception(exception_unexpected));
    }
}; /// class threadtt
typedef threadtt<> thread;

namespace crt {
namespace internal {
typedef unsigned (__stdcall *StartAddress)(void*);
typedef windows::internal::threadt<StartAddress, void*, unsigned> thread;
} /// namespace internal

/// class threadtt
template <class TExtends = threadt<internal::thread>, class TImplements = typename TExtends::implements>
class exported threadtt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadtt derives; 
    
    typedef typename extends::StartAddress StartAddress;
    typedef typename extends::Parameter Parameter;

    /// constructors / destructor
    threadtt(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded): extends(startAddress, parameter, initiallySuspeneded) {
    }
    threadtt(StartAddress startAddress, Parameter parameter): extends(startAddress, parameter) {
    }
    virtual ~threadtt() {
    }
private:
    threadtt(const threadtt& copy): extends(0, 0) {
        throw (exception(exception_unexpected));
    }
}; /// class threadtt
typedef threadtt<> thread;
} /// namespace crt

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#endif /// XOS_PLATFORM_OS_MICROSOFT_WINDOWS_THREAD_HPP
