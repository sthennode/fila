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
///   File: semaphore.hpp
///
/// Author: $author$
///   Date: 1/6/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP
#define XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP

#include "xos/mt/semaphore.hpp"

#if !defined(WINDOWS)
#include "xos/platform/os/microsoft/windows/Semaphore.h"
#endif /// !defined(WINDOWS)

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

/// class semaphoret
template 
<class TExtends = mt::extended::semaphoret<HANDLE>, 
 class TImplements = typename TExtends::implements>
class exported semaphoret: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef semaphoret derives;
    
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructor / destructor
    semaphoret(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    semaphoret(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    semaphoret(attached_t detached): extends(detached) {
    }
    semaphoret(bool is_logged): extends(is_logged) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    semaphoret(const semaphoret& copy): extends(copy) {
    }
    semaphoret() {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~semaphoret() {
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw (create_exception(destroy_failed));
        }
    }

    /// ...acquire... / ...release...
    virtual acquire_status timed_acquire(mseconds_t milliseconds) {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return timed_acquire_detached(detached, milliseconds);
        }
        return acquire_failed;
    }
    virtual acquire_status untimed_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return untimed_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual acquire_status try_acquire() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return try_acquire_detached(detached);
        }
        return acquire_failed;
    }
    virtual bool release() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            return release_detached(detached);
        }
        return false;
    }

    /// ...acquire_detached... / ...release_detached...
    virtual bool acquire_detached(attached_t detached) const { 
        return (acquire_success == (untimed_acquire_detached(detached))); 
    }
    virtual acquire_status timed_acquire_detached(attached_t detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
            if ((attached_t)(unattached) != (detached)) {
                bool debug = this->is_logged_debug(milliseconds), logged = this->is_logged();
                DWORD dwMilliseconds = ((DWORD)milliseconds), dwResult = 0;
                LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")...");
                if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                    LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_success;
                } else {
                    switch (dwResult) {
                    case WAIT_TIMEOUT:
                        LOGGER_IF_LOGGED_DEBUG_TRACE(debug, logged, "...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                        return acquire_busy;
                    case WAIT_ABANDONED:
                        LOGGER_IS_LOGGED_ERROR("...failed WAIT_ABANDONED on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                        return acquire_interrupted;
                    default:
                        LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    }
                }
            }
        } else {
            if (0 > (milliseconds)) {
                return untimed_acquire_detached(detached);
            } else {
                return try_acquire_detached(detached);
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status try_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            DWORD dwMilliseconds = 0, dwResult = 0;
            LOGGER_IS_LOGGED_TRACE("::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")...");
            if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                LOGGER_IS_LOGGED_TRACE("...::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                return acquire_success;
            } else {
                switch (dwResult) {
                case WAIT_TIMEOUT:
                    LOGGER_IS_LOGGED_TRACE("...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_busy;
                case WAIT_ABANDONED:
                    LOGGER_IS_LOGGED_ERROR("...failed WAIT_ABANDONED on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_interrupted;
                default:
                    LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                }
            }
        }
        return acquire_failed; 
    }
    virtual acquire_status untimed_acquire_detached(attached_t detached) const { 
        if ((attached_t)(unattached) != (detached)) {
            DWORD dwMilliseconds = INFINITE, dwResult = 0;
            LOGGER_IS_LOGGED_DEBUG("::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")...");
            if ((WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds)))) {
                LOGGER_IS_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                return acquire_success;
            } else {
                switch (dwResult) {
                case WAIT_TIMEOUT:
                    LOGGER_IS_LOGGED_DEBUG("...failed WAIT_TIMEOUT on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_busy;
                case WAIT_ABANDONED:
                    LOGGER_IS_LOGGED_ERROR("...failed WAIT_ABANDONED on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                    return acquire_interrupted;
                default:
                    LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds = " << dwMilliseconds << ")");
                }
            }
        }
        return acquire_failed; 
    }
    virtual bool release_detached(attached_t detached) const { 
        return release_detached(detached, 1);
    }
    virtual bool release_detached(attached_t detached, size_t count) const { 
        if ((attached_t)(unattached) != (detached)) {
            if (0 < (count)) {
                LONG lReleaseCount = ((LONG)count), lPreviousCount = 0;
                LOGGER_IS_LOGGED_DEBUG("::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)...");
                if ((::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount))) {
                    LOGGER_IS_LOGGED_DEBUG("...::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)");
                    return true;
                } else {
                    int error = GetLastError();
                    LOGGER_IS_LOGGED_ERROR("...failed error = " << error << " on ::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)");
                }
            } else {
                return true;
            }
        }
        return false; 
    }

    /// ...create... / ...destroy...
    using extends::create_attached;
    virtual attached_t create_attached(size_t initially_released) {
        LONG lInitialCount = ((LONG)initially_released);
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(lInitialCount)))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    virtual attached_t create_detached() const {
        LONG lInitialCount = 0, lMaximumCount = (((ULONG)(-1)) >> 1);
        return create_detached(lInitialCount, lMaximumCount);
    }
    virtual attached_t create_detached(LONG lInitialCount) const {
        LONG lMaximumCount = (((ULONG)(-1)) >> 1);
        return create_detached(lInitialCount, lMaximumCount);
    }
    virtual attached_t create_detached(LONG lInitialCount, LONG lMaximumCount) const {
        LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
        LPCTSTR lpName = 0;
        attached_t detached = (attached_t)(unattached);
        LOGGER_IS_LOGGED_DEBUG("::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)...");
        if ((attached_t)(unattached) != (detached = ::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName))) {
            LOGGER_IS_LOGGED_DEBUG("...::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)");
        } else {
            int error = GetLastError();
            LOGGER_IS_LOGGED_ERROR("...failed error = " << error << " on ::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)");
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
}; /// class semaphoret
typedef semaphoret<> semaphore;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// ndef XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HPP 
