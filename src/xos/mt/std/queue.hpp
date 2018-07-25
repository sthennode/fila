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
///   File: queue.hpp
///
/// Author: $author$
///   Date: 7/12/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_STD_QUEUE_HPP
#define _XOS_MT_STD_QUEUE_HPP

#include "xos/mt/os/semaphore.hpp"
#include "xos/mt/os/mutex.hpp"
#include "xos/base/locked.hpp"
#include <queue>

namespace xos {
namespace mt {
namespace std {

///////////////////////////////////////////////////////////////////////
///  Class: queuet
///////////////////////////////////////////////////////////////////////
template
<typename TItem,
 class TImplements = implement_base, class TExtends = base>

class _EXPORT_CLASS queuet: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef TItem item_t;

    queuet(): cleared_(false) {
    }
    virtual ~queuet() {
        clear();
    }

    virtual void clear() {
        lock l(mutex_);
        while (0 < (queue_.size())) {
            item_t item = queue_.front();
            cleared(item, queue_.front());
            queue_.pop();
        }
        cleared_ = true;
    }
    virtual void unclear() {
        lock l(mutex_);
        cleared_ = false;
    }

    virtual bool enqueue(item_t& item) {
        lock l(mutex_);
        queue_.push(item);
        if (0 < (queue_.size())) {
            enqueued(item, queue_.back());
            semaphore_.release();
            return true;
        } else {
            LOG_ERROR("...unexpected 0 >= (queue_.size())");
        }
        return false;
    }
    virtual bool dequeue(item_t& item) {
        if ((semaphore_.acquire())) {
            lock l(mutex_);
            if (0 < (queue_.size())) {
                item = queue_.front();
                dequeued(item, queue_.front());
                queue_.pop();
                return true;
            } else {
                if ((cleared_)) {
                    semaphore_.release();
                }
            }
        } else {
            LOG_ERROR("...failed on semaphore_.acquire()");
        }
        return false;
    }
    virtual bool try_dequeue(item_t& item) {
        if ((semaphore_.try_acquire())) {
            lock l(mutex_);
            if (0 < (queue_.size())) {
                item = queue_.front();
                dequeued(item, queue_.front());
                queue_.pop();
                return true;
            } else {
                if ((cleared_)) {
                    semaphore_.release();
                }
            }
        } else {
        }
        return false;
    }

protected:
    virtual void cleared(item_t& item, item_t& front) {
    }
    virtual void enqueued(item_t& item, item_t& back) {
    }
    virtual void dequeued(item_t& item, item_t& front) {
    }

protected:
    bool cleared_;
    os::mutex mutex_;
    os::semaphore semaphore_;
    ::std::queue<item_t> queue_;
};

} /// namespace std
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_STD_QUEUE_HPP 
