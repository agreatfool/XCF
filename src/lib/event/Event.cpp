#include "Event.h"

namespace XCF {

    EventIo::EventIo():
        ioLoop(
#if defined (__APPLE__)
            ev_loop_new(EVBACKEND_KQUEUE)
#elif defined (__linux)
            ev_loop_new(EVBACKEND_EPOLL)
#endif
        ), ioWatcherPool(new EventWatcherMap()) {}

    EventIo::~EventIo() {
        ev_suspend(this->ioLoop);
        if (this->ioWatcherPool->size() > 0) {
            EventWatcherIterator it;
            for (it = this->ioWatcherPool->begin(); it != this->ioWatcherPool->end(); /* no auto increment*/) {
                EventWatcher *watcher = it->second;
                ev_io_stop(this->ioLoop, watcher);
                this->ioWatcherPool->erase(it++);
            }
        }
        ev_break(this->ioLoop, EVBREAK_ALL);
        ev_loop_destroy(this->ioLoop);
        delete this->ioWatcherPool;
//        delete this->ioLoop; // FIXME should we delete the "this->ioLoop" ?
/*
In compiling:
../src/lib/event/Event.cpp:27: warning: possible problem detected in invocation of delete operator:
../src/lib/event/Event.cpp:27: warning: invalid use of incomplete type 'struct ev_loop'
/opt/local/include/ev.h:166: warning: forward declaration of 'struct ev_loop'
*/
    }

    void EventIo::stopLoop() {
        this->clearWatchers();
        ev_break(this->ioLoop, EVBREAK_ALL);
    }

    void EventIo::addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventWatcher *watcher, int revents)) {
        EventWatcher *watcher = (EventWatcher *) malloc(sizeof(EventWatcher));
        ev_io_init(watcher, callback, socketFd, EV_READ);
        ev_io_start(this->ioLoop, watcher);
        this->ioWatcherPool->insert(EventWatcherMap::value_type(socketFd, watcher));
    }

    EventWatcher *EventIo::getWatcher(int32_t socketFd) {
        EventWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->end()) {
            return it->second;
        } else {
            return NULL;
        }
    }

    void EventIo::removeWatcher(int32_t socketFd) {
        EventWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->end()) {
            // found the watcher in the pool
            ev_io_stop(this->ioLoop, it->second);
            this->ioWatcherPool->erase(it);
        }
    }

    void EventIo::clearWatchers() {
        if (this->ioWatcherPool->size() > 0) {
            EventWatcherIterator it;
            for (it = this->ioWatcherPool->begin(); it != this->ioWatcherPool->end(); /* no auto increment*/) {
                EventWatcher *watcher = it->second;
                ev_io_stop(this->ioLoop, watcher);
                delete it->second;
                this->ioWatcherPool->erase(it++);
            }
            this->ioWatcherPool->clear();
        }
    }

} /* namespace XCF */
