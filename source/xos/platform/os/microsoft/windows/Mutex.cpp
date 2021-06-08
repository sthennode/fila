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
///   File: Mutex.cpp
///
/// Author: $author$
///   Date: 1/3/2020
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/microsoft/windows/Mutex.hpp"
#include "xos/platform/os/microsoft/windows/Handle.hpp"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

typedef Handle MutexExtends;
typedef MutexExtends::implements MutexImplements;
/// class Mutex
class exported Mutex: virtual public MutexImplements, public MutexExtends {
public:
    typedef MutexImplements implements;
    typedef MutexExtends extends;
    typedef Mutex derives;

    typedef ::xos::mt::os::mutex::attached_t attached_t;
    typedef ::xos::mt::os::mutex::unattached_t unatached_t;
    enum { unattached = ::xos::mt::os::mutex::unattached };
    
    /// constructor / destructor
    Mutex(BOOL bInitialOwner): mutex_(((attached_t)unattached), false, false) {
        bool initially_locked = (bInitialOwner != FALSE);
        if (!(mutex_.create(initially_locked))) {
            throw create_exception(create_failed);
        }
    }
    Mutex(): mutex_(((attached_t)unattached), false, false) {
        if (!(mutex_.create())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~Mutex() {
    }
private:
    Mutex(const Mutex& copy) {
    }

public:
    /// Release...
    virtual BOOL ReleaseMutex() {
        BOOL success = FALSE;
        if ((mutex_.unlock())) {
            success = TRUE;
        }
        return success;
    }

    /// Wait... / Close...
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::lock_status status = ::xos::lock_failed;
        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = mutex_.timed_lock(dwMilliseconds);
            } else {
                status = mutex_.try_lock();
            }
        } else {
            status = mutex_.untimed_lock();
        }
        switch (status) {
        case ::xos::lock_success:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::lock_busy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::lock_interrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual BOOL CloseHandle() {
        BOOL success = (mutex_.destroy())?(TRUE):(FALSE);
        return success;
    }
    
    /// To...
    virtual Mutex* ToMutex() const {
        return ((Mutex*)this);
    }

protected:
    ::xos::mt::os::mutex mutex_;
}; /// class Mutex

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
///
/// windows mutexes
/// ...
HANDLE WINAPI CreateMutex(
  _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
  _In_     BOOL                  bInitialOwner,
  _In_opt_ LPCTSTR               lpName
) {
    try {
        bool initiallyLocked = (bInitialOwner != FALSE);
        ::xos::platform::os::microsoft::windows::Mutex* mutex = 0;
        if ((mutex = new ::xos::platform::os::microsoft::windows::Mutex(initiallyLocked))) {
            return mutex;
        }
    } catch (...) {
    }
    return NULL_HANDLE;
}
BOOL WINAPI ReleaseMutex(
  _In_ HANDLE hMutex
) {
    ::xos::platform::os::microsoft::windows::Handle* handle = 0;
    if ((handle = ((::xos::platform::os::microsoft::windows::Handle*)hMutex))) {
        ::xos::platform::os::microsoft::windows::Mutex* mutex = 0;
        if ((mutex = handle->ToMutex())) {
            BOOL success = FALSE;
            success = mutex->ReleaseMutex();
            return success;
        }
    }
    return FALSE;
}
/// ...
/// windows mutexes
///
#endif /// !defined(WINDOWS)
