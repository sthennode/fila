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
///   File: thread.hpp
///
/// Author: $author$
///   Date: 9/30/2020
///////////////////////////////////////////////////////////////////////
#ifndef XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP
#define XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP

#include "xos/mt/thread.hpp"

#if defined(WINDOWS)
#include <process.h>
#include <errno.h>
#else /// defined(WINDOWS)
#include "xos/platform/os/microsoft/windows/Thread.h"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {
namespace crt {

/// class threadt
template 
<typename TThread = HANDLE,
 class TExtends = mt::derive::threadt<TThread, int, 0, -1, EBUSY, ETIMEDOUT, EINTR>, 
 class TImplements = typename TExtends::implements>
class exported threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives; 
    
    typedef TThread thread_t;
    typedef typename extends::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    /// constructors / destructor
    threadt(ran_t &ran, attached_t detached, bool is_created, bool is_logged): extends(ran, detached, is_created, is_logged), thread_id_(0) {
    }
    threadt(ran_t &ran, attached_t detached, bool is_created): extends(ran, detached, is_created), thread_id_(0) {
    }
    threadt(ran_t &ran, attached_t detached): extends(ran, detached), thread_id_(0) {
    }
    threadt(ran_t &ran, bool is_logged): extends(ran, is_logged), thread_id_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(ran_t &ran): extends(ran), thread_id_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged), thread_id_(0) {
    }
    threadt(attached_t detached, bool is_created): extends(detached, is_created), thread_id_(0) {
    }
    threadt(attached_t detached): extends(detached), thread_id_(0) {
    }
    threadt(bool is_logged): extends(is_logged), thread_id_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    threadt(const threadt& copy): thread_id_(0) {
        LOGGER_IS_LOGGED_ERROR("...unexpected throw (exception(exception_unexpected))...");
        throw (exception(exception_unexpected));
    }
    threadt(): thread_id_(0) {
        if (!(this->create())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->create() throw (create_exception(create_failed))...");
            throw (create_exception(create_failed));
        }
    }
    virtual ~threadt() {
        if (!(this->joined())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->joined() throw (join_exception(join_failed))...");
            throw join_exception(join_failed);
        }
        if (!(this->destroyed())) {
            LOGGER_IS_LOGGED_ERROR("...failed on this->destroyed() throw (create_exception(destroy_failed))...");
            throw create_exception(destroy_failed);
        }
    }
    
    /// create_attached
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if (((attached_t)(unattached) != (detached = create_detached(thread_id_, this->thread_)))) {
                this->attach(detached);
            }
        }
        return detached;
    }

    /// ...join_detached
    virtual join_status try_join_detached(bool& forked, HANDLE& detached) const {
        return timed_join_detached(forked, detached, 0);
    }
    virtual join_status untimed_join_detached(bool& forked, HANDLE& detached) const {
        if ((detached)) {
            DWORD dwMilliseconds = ((DWORD)INFINITE);
            DWORD dwResult = 0;

            LOGGER_IS_LOGGED_DEBUG("::WaitForSingleObject(detached, dwMilliseconds)...");
            if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                switch(dwResult) {
                case WAIT_TIMEOUT:
                    LOGGER_IS_LOGGED_ERROR("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return join_busy;
                case WAIT_ABANDONED:
                    LOGGER_IS_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return join_interrupted;
                default:
                    LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    forked = false;
                    return join_failed;
                }
            } else {
                LOGGER_IS_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                forked = false;
                return join_success;
            }
        }
        return join_failed;
    }
    virtual join_status timed_join_detached(bool& forked, HANDLE& detached, mseconds_t milliseconds) const {
        if (0 > (milliseconds)) {
            return untimed_join_detached(forked, detached);
        } else {
            if ((detached)) {
                DWORD dwMilliseconds = ((DWORD)milliseconds);
                DWORD dwResult = 0;
    
                LOGGER_IS_LOGGED_TRACE("::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    switch(dwResult) {
                    case WAIT_TIMEOUT:
                        LOGGER_IS_LOGGED_TRACE("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return join_busy;
                    case WAIT_ABANDONED:
                        LOGGER_IS_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return join_interrupted;
                    default:
                        LOGGER_IS_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        forked = false;
                        return join_failed;
                    }
                } else {
                    LOGGER_IS_LOGGED_TRACE("...::WaitForSingleObject(detached, dwMilliseconds)");
                    forked = false;
                    return join_success;
                }
            }
        }
        return join_failed;
    }

    /// create_detached / destroy_detached
    virtual attached_t create_detached(unsigned& thread_id, HANDLE& thread) const {
        return create_detached(thread_id, thread, false);
    }
    virtual attached_t create_detached(unsigned& thread_id, HANDLE& thread, bool initially_suspended) const {
        attached_t detached = (attached_t)(unattached);
        unsigned (__stdcall*startAddress)(void*) = thread_proc;
        void* parameter = ((void*)this);
        unsigned initFlags = (initially_suspended)?(CREATE_SUSPENDED):(0);
        unsigned stackSize = 0;
        SECURITY_ATTRIBUTES securityAttributes;

        ::memset(&securityAttributes, 0, sizeof(securityAttributes));
        securityAttributes.nLength = sizeof(securityAttributes);

        LOGGER_IS_LOGGED_DEBUG("::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)...");
        if (((HANDLE)0) != (thread = ((HANDLE)::_beginthreadex
            (&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)))) {
            detached = &thread;
            return detached;
        } else {
            int err = errno;
            switch(err) {
            case EACCES:
                LOGGER_IS_LOGGED_ERROR("...EACCES err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EAGAIN:
                LOGGER_IS_LOGGED_ERROR("...EAGAIN err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EINVAL:
                LOGGER_IS_LOGGED_ERROR("...EINVAL err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            default:
                LOGGER_IS_LOGGED_ERROR("...failed err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
            }
            detached = (attached_t)(unattached);
        }
        return detached;
    }
    virtual bool destroy_detached(HANDLE& detached) const {
        if ((attached_t)(unattached) != (detached)) {
            LOGGER_IS_LOGGED_DEBUG("::CloseHandle(detached = " << pointer_to_string(detached) << ")...");
            if ((::CloseHandle(detached))) {
                return true;
            } else {
                DWORD dwError = ::GetLastError();
                LOGGER_IS_LOGGED_ERROR("...failed dwError = " << dwError << " on ::CloseHandle(detached = " << pointer_to_string(detached) << ")");
            }
        }
        return false;
    }
    
protected:
    /// run
    virtual pointer_t run(pointer_t parameter) {
        pointer_t result = 0;
        LOGGER_IS_LOGGED_DEBUG("...return result = " << pointer_to_string(result) << "...");
        return result;
    }

    /// thread_proc
    static unsigned __stdcall thread_proc(void* param) {
        unsigned result = 0;
        threadt* t = 0;
        if ((t = ((threadt*)param))) {
            LOGGER_IF_LOGGED_DEBUG(t->is_logged(), "t->ran_.run()...");
            t->ran_.run();
            LOGGER_IF_LOGGED_DEBUG(t->is_logged(), "...t->ran_.run()");
        } else {
            LOGGER_LOG_ERROR("...unexpected 0 = ((threadt*)param)");
        }
        return result;
    }

protected:
    unsigned thread_id_;
}; /// class threadt
typedef threadt<> thread;

} /// namespace crt
} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HPP
