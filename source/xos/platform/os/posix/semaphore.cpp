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
///   File: semaphore.cpp
///
/// Author: $author$
///   Date: 1/7/2020
///////////////////////////////////////////////////////////////////////
#include "xos/mt/os/semaphore.hpp"
#include "xos/platform/os/posix/semaphore.hpp"

namespace xos {
namespace platform {
namespace os {
namespace posix {

typedef mt::os::semaphore semaphore;

} /// namespace posix
} /// namespace os
} /// namespace platform
} /// namespace xos

///
/// posix semaphores
/// ...
int sem_init(sem_t *sem, int pshared, unsigned int value) {
    if ((sem)) {
        if (!(pshared) && !(value)) {
            ::xos::platform::os::posix::semaphore* semaphore = 0;
            try {
                if ((semaphore = new ::xos::platform::os::posix::semaphore(((::xos::platform::os::posix::semaphore::attached_t)::xos::platform::os::posix::semaphore::unattached), false, false))) {
                    int err = EFAULT;
                    if ((semaphore->create())) {
                        *sem = ((sem_t)semaphore);
                        return 0;
                    }
                    try {
                        delete semaphore;
                    } catch (...) {
                        return ENOMEM;
                    }
                    return err;
                }
            } catch (...) {
                return ENOMEM;
            }
        }
    }
    return EINVAL;
}
int sem_destroy(sem_t *sem) {
    if ((sem) && (*sem)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            int err = EFAULT;
            if ((semaphore->destroy())) {
                err = 0;
            }
            *sem = 0;
            try {
                delete semaphore;
            } catch (...) {
                return ENOMEM;
            }
            return err;
        }
    }
    return EINVAL;
}

sem_t *sem_open(const char *name, int oflag, ...) {
    return 0;
}
int sem_close(sem_t *sem) {
    return EINVAL;
}
int sem_unlink(const char *name) {
    return EINVAL;
}

int sem_post(sem_t *sem) {
    if ((sem) && (*sem)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            int err = EFAULT;
            if ((semaphore->release())) {
                err = 0;
            }
            return err;
        }
    }
    return EINVAL;
}
int sem_wait(sem_t *sem) {
    if ((sem) && (*sem)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            int err = EFAULT;
            if ((semaphore->acquire())) {
                err = 0;
            }
            return err;
        }
    }
    return EINVAL;
}
int sem_trywait(sem_t *sem) {
    if ((sem) && (*sem)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            ::xos::acquire_status status = ::xos::acquire_failed;
            int err = EFAULT;
            if (::xos::acquire_success == (status = semaphore->try_acquire())) {
                errno = err = 0;
            } else {
                if (::xos::acquire_busy == (status)) {
                    errno = err = EBUSY;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        errno = err = ETIMEDOUT;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            errno = err = EINTR;
                        }
                    }
                }
            }
            return err;
        }
    }
    return EINVAL;
}
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    if ((sem) && (*sem) && (abs_timeout)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            mseconds_t abs_milliseconds = ::xos::seconds_mseconds(abs_timeout->tv_sec) + ::xos::nseconds_mseconds(abs_timeout->tv_nsec);
            int err = EFAULT;
            struct timespec timeout;
            if (!(err = clock_gettime(CLOCK_REALTIME, &timeout))) {
                mseconds_t milliseconds = ::xos::seconds_mseconds(timeout.tv_sec) + ::xos::nseconds_mseconds(timeout.tv_nsec);
                ::xos::acquire_status status = ::xos::acquire_failed;
                if (::xos::acquire_success == (status = semaphore->timed_acquire(((abs_milliseconds >= milliseconds)?(abs_milliseconds - milliseconds):(0))))) {
                    errno = err = 0;
                } else {
                    if (::xos::acquire_busy == (status)) {
                        errno = err = EBUSY;
                    } else {
                        if (::xos::acquire_timeout == (status)) {
                            errno = err = ETIMEDOUT;
                        } else {
                            if (::xos::acquire_interrupted == (status)) {
                                errno = err = EINTR;
                            }
                        }
                    }
                }
            }
            return err;
        }
    }
    return EINVAL;
}
int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
    if ((sem) && (*sem) && (timeout)) {
        ::xos::platform::os::posix::semaphore* semaphore = 0;
        if ((semaphore = ((::xos::platform::os::posix::semaphore*)(*sem)))) {
            mseconds_t milliseconds = ::xos::seconds_mseconds(timeout->tv_sec) + ::xos::nseconds_mseconds(timeout->tv_nsec);
            ::xos::acquire_status status = ::xos::acquire_failed;
            int err = EFAULT;
            if (::xos::acquire_success == (status = semaphore->timed_acquire(milliseconds))) {
                errno = err = 0;
            } else {
                if (::xos::acquire_busy == (status)) {
                    errno = err = EBUSY;
                } else {
                    if (::xos::acquire_timeout == (status)) {
                        errno = err = ETIMEDOUT;
                    } else {
                        if (::xos::acquire_interrupted == (status)) {
                            errno = err = EINTR;
                        }
                    }
                }
            }
            return err;
        }
    }
    return EINVAL;
}
/// ...
/// poxix semaphores
/// 
