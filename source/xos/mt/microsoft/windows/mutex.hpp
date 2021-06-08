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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 1/2/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP
#define XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP

#include "xos/mt/mutex.hpp"

#if !defined(WINDOWS)
#include "xos/platform/os/microsoft/windows/Mutex.h"
#endif /// !defined(WINDOWS)

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

/// class mutext
template 
<typename TAttached = HANDLE,
 class TExtends = mt::extended::mutext<TAttached>, 
 class TImplements = typename TExtends::implements>
class exported mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef mutext derives;
    
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    mutext(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    mutext(const mutext& copy): extends(copy) {
    }
    mutext() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~mutext() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// ...lock... / ...unlock...
    virtual lock_status time_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return time_lock_detached(detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_lock_detached(detached, milliseconds);
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_lock_detached(detached);
        }
        return lock_failed;
    }
    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_lock_detached(detached);
        }
        return lock_failed;
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return unlock_detached(detached);
        }
        return false;
    }

    /// ...lock_detached... / ...unlock_detached...
    virtual bool lock_detached(attached_t detached) const { 
        return (lock_success == (untimed_lock_detached(detached))); 
    }
    virtual lock_status time_lock_detached(attached_t detached, mseconds_t milliseconds) const { 
        if ((attached_t)(unattached) != (detached)) {
        }
        return lock_failed; 
    }
    virtual lock_status timed_lock_detached(attached_t detached, mseconds_t milliseconds) const { 
        if ((attached_t)(unattached) != (detached)) {
            bool debug = this->is_logged_debug(milliseconds),
                 logged = this->is_logged();
            DWORD dwMilliseconds = (0 > milliseconds)?(INFINITE):((DWORD)milliseconds), 
                  dwResult = 0;
            LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "::WaitForSingleObject(detached, dwMilliseconds)...");
            if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...::WaitForSingleObject(detached, dwMilliseconds)");
                return lock_success;
            } else {
                if (WAIT_TIMEOUT == (dwResult)) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                    return lock_busy;
                } else {
                    if (WAIT_ABANDONED == (dwResult)) {
                        LOGGER_IS_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                        return lock_interrupted;
                    } else {
                        LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                    }
                }
            }
        }
        return lock_failed; 
    }
    virtual lock_status try_lock_detached(attached_t detached) const { 
        return timed_lock_detached(detached, 0); 
    }
    virtual lock_status untimed_lock_detached(attached_t detached) const { 
        return timed_lock_detached(detached, -1); 
    }
    virtual bool unlock_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            LOGGER_IS_LOGGED_DEBUG("::ReleaseMutex(detached)...");
            if ((::ReleaseMutex(detached))) {
                LOGGER_IS_LOGGED_DEBUG("...::ReleaseMutex(detached)");
                return true;
            } else {
                DWORD error = ::GetLastError();
                LOGGER_IS_LOGGED_ERROR("...failed error = " << error << " on ::ReleaseMutex(detached)");
            }
        }
        return false; 
    }

    /// ...create... / ...destroy...
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached()))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached() const {
        attached_t detached = create_detached(FALSE);
        return detached;
    }
    virtual attached_t create_detached(BOOL bInitialOwner) const {
        LPCTSTR lpName = NULL;
        LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL;
        attached_t detached = (attached_t)(unattached);
        LOGGER_IS_LOGGED_DEBUG("::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)...");
        if ((detached = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName))) {
            LOGGER_IS_LOGGED_DEBUG("...::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)");
        } else {
            DWORD error = ::GetLastError();
            LOGGER_IS_LOGGED_ERROR("...failed error = " << error << " on ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)");
        }
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
            bool success = true;
            LOGGER_IS_LOGGED_DEBUG("::CloseHandle(detached = " << pointer_to_string(detached) << ")...");
            if (!(::CloseHandle(detached))) {
                DWORD error = ::GetLastError();
                LOGGER_IS_LOGGED_ERROR("...failed error = " << error << " on ::CloseHandle(detached = " << pointer_to_string(detached) << ")");
                success = false;
            }
            return success;
        }
        return false;
    }
}; /// class mutext
typedef mutext<> mutex;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_MICROSOFT_WINDOWS_MUTEX_HPP 
