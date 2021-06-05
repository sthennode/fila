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
///   File: mutex.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_MUTEX_HPP
#define _XOS_MT_POSIX_MUTEX_HPP

#include "xos/mt/posix/timed.hpp"
#include "xos/mt/mutex.hpp"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#else /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#if !defined(pthread_mutex_timedlock)
#define pthread_mutex_timedlock(m, t) EINVAL
#endif /// !defined(pthread_mutex_timedlock)
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )

namespace xos {
namespace mt {
namespace posix {

typedef pthread_mutexattr_t mutexattr_t;
typedef pthread_mutex_t mutex_t;
typedef mutex_t* mutex_attached_t;

typedef mt::mutex mutext_implements;
typedef mt::extended::mutext<mutex_attached_t> mutext_extends;
///////////////////////////////////////////////////////////////////////
///  Class: mutext
///////////////////////////////////////////////////////////////////////
template <class TImplements = mutext_implements, class TExtends = mutext_extends>
class _EXPORT_CLASS mutext: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };
    
    mutext(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    mutext(attached_t detached): extends(detached) {
    }
    mutext(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(bool is_logged): extends(is_logged) {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    mutext(const mutext &copy): extends(copy) {
    }
    mutext() {
        IS_ERR_LOGGED_DEBUG("this->created()...");
        if (!(this->created())) {
            const create_exception e(create_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->created() throw (const create_exception e(create_failed))...");
            throw (e);
        }
    }
    virtual ~mutext() {
        IS_ERR_LOGGED_DEBUG("this->destroyed()...");
        if (!(this->destroyed())) {
            const create_exception e(destroy_failed);
            IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw (const create_exception e(destroy_failed))...");
            throw (e);
        }
    }

    virtual bool create() {
        if ((this->destroyed())) {
            attached_t detached = 0;
            if ((detached = this->create_detached(mutex_))) {
                this->attach_created(detached);
                return true;
            }
        }
        return false;
    }
    virtual bool destroy() {
        attached_t detached = 0;
        if ((detached = this->detach())) {
            if ((this->destroy_detached(*detached))) {
                return true;
            }
        }
        return false;
    }

    virtual bool lock() {
        return lock_success == untimed_lock();
    }
    virtual bool unlock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            IS_LOGGED_DEBUG("pthread_mutex_unlock(detached)...");
            if (!(err = pthread_mutex_unlock(detached))) {
                return true;
            } else {
                IS_LOGGED_ERROR("...failed err =" << err << " on pthread_mutex_unlock(detached)");
            }
        }
        return false;
    }

    virtual lock_status try_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            IS_LOGGED_TRACE("pthread_mutex_trylock(detached)...");
            if (!(err = pthread_mutex_trylock(detached))) {
                IS_LOGGED_TRACE("...pthread_mutex_trylock(detached)");
                return lock_success;
            } else {
                if (ETIMEDOUT != (err)) {
                    if (EINTR != (err)) {
                        IS_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_trylock(detached)");
                    } else {
                        IS_LOGGED_ERROR("...failed EINTR err = " << err << " on pthread_mutex_trylock(detached)");
                        return lock_interrupted;
                    }
                } else {
                    IS_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on pthread_mutex_trylock(detached)");
                    return lock_busy;
                }
            }
        }
        return lock_failed;
    }
    virtual lock_status untimed_lock() {
        attached_t detached = 0;
        if ((detached = this->attached_to())) {
            int err = 0;
            IS_LOGGED_DEBUG("pthread_mutex_lock(detached)...");
            if (!(err = pthread_mutex_lock(detached))) {
                IS_LOGGED_DEBUG("...pthread_mutex_lock(detached)");
                return lock_success;
            } else {
                if (EINTR != (err)) {
                    IS_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_lock(detached)");
                } else {
                    IS_LOGGED_ERROR("...failed EINTR err = " << err << " on pthread_mutex_lock(detached)");
                    return lock_interrupted;
                }
            }
        }
        return lock_failed;
    }
    virtual lock_status timed_lock(mseconds_t milliseconds) {
        lock_status status = lock_failed;
        if (0 > milliseconds) {
            status = untimed_lock();
        } else {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            attached_t detached = 0;
            if ((detached = this->attached_to())) {
                struct timespec until_time = timed_until_time(milliseconds);
                int err = 0;

                if (500 > milliseconds) {
                    IS_LOGGED_TRACE("pthread_mutex_timedlock(detached, &until_time)...");
                } else {
                    IS_LOGGED_DEBUG("pthread_mutex_timedlock(detached, &until_time)...");
                }
                if (!(err = pthread_mutex_timedlock(detached, &until_time))) {
                    if (500 > milliseconds) {
                        IS_LOGGED_TRACE("...pthread_mutex_timedlock(detached, &until_time)");
                    } else {
                        IS_LOGGED_DEBUG("...pthread_mutex_timedlock(detached, &until_time)");
                    }
                    return lock_success;
                } else {
                    if (ETIMEDOUT != (err)) {
                        if (EINTR != (err)) {
                            IS_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_timedlock(detached, &until_time)");
                        } else {
                            IS_LOGGED_ERROR("...failed EINTR err = " << err << " on pthread_mutex_timedlock(detached, &until_time)");
                            return lock_interrupted;
                        }
                    } else {
                        if (500 > milliseconds) {
                            IS_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on pthread_mutex_timedlock(detached, &until_time)");
                        } else {
                            IS_LOGGED_DEBUG("...failed ETIMEDOUT err = " << err << " on pthread_mutex_timedlock(detached, &until_time)");
                        }
                        return lock_busy;
                    }
                }
            }
#else // defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            if (milliseconds) {
                IS_LOGGED_ERROR("...invalid pthread_mutex_timedlock(detached, ...)");
                status = lock_invalid;
            } else {
                status = this->try_lock();
            }
#endif // defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
        }
        return status;
    }

    virtual attached_t create_detached(mutex_t& mutex) const {
        int err = 0;
        mutexattr_t mutexattr;
        IS_ERR_LOGGED_DEBUG("pthread_mutexattr_init(&mutexattr)...");
        if (!(err = pthread_mutexattr_init(&mutexattr))) {
            IS_ERR_LOGGED_DEBUG("pthread_mutex_init(&mutex, &mutexattr)...");
            if (!(err = pthread_mutex_init(&mutex, &mutexattr))) {
                IS_ERR_LOGGED_DEBUG("pthread_mutexattr_destroy(&mutexattr)...");
                if (!(err = pthread_mutexattr_destroy(&mutexattr))) {
                    return &mutex;
                } else {
                    IS_ERR_LOGGED_ERROR("...failed err =" << err << " on pthread_mutexattr_destroy(&mutexattr)");
                    IS_ERR_LOGGED_DEBUG("pthread_mutex_destroy(&mutex)...");
                    if ((err = pthread_mutex_destroy(&mutex))) {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_destroy(&mutex)");
                    }
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_init(&mutex, &mutexattr)");
                IS_ERR_LOGGED_DEBUG("pthread_mutexattr_destroy(&mutexattr)...");
                if ((err = pthread_mutexattr_destroy(&mutexattr))) {
                    IS_ERR_LOGGED_ERROR("...failed err =" << err << " on pthread_mutexattr_destroy(&mutexattr)");
                }
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed err =" << err << " on pthread_mutexattr_init(&mutexattr)");
        }
        return 0;
    }
    virtual bool destroy_detached(mutex_t& mutex) const {
        bool success = true;
        int err = 0;
        IS_ERR_LOGGED_DEBUG("pthread_mutex_destroy(&mutex)...");
        if ((err = pthread_mutex_destroy(&mutex))) {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on pthread_mutex_destroy(&mutex)");
            success = false;
        }
        return success;
    }

protected:
    mutex_t mutex_;
};
typedef mutext<> mutex;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_MUTEX_HPP 
