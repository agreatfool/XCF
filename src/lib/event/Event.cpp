#include "Event.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- Event
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Event::Event():
        ioLoop(
#if defined (__APPLE__)
            ev_loop_new(EVBACKEND_KQUEUE | EVFLAG_NOENV)
#elif defined (__linux)
            ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV)
#endif
        ) {};

    Event::~Event() {
        this->stopLoop();
        ev_loop_destroy(this->ioLoop);
    };

    void Event::stopLoop() {
        this->suspendLoop();
        this->clearWatchers();
        ev_break(this->ioLoop, EVBREAK_ALL);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventIo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo::EventIo(): Event(), ioWatcherPool(new EventIoWatcherMap()) {}

    EventIo::~EventIo() {
        delete this->ioWatcherPool;
//        delete this->ioLoop;
// FIXME should we delete the "this->ioLoop" ?
/*
In compiling:
../src/lib/event/Event.cpp:27: warning: possible problem detected in invocation of delete operator:
../src/lib/event/Event.cpp:27: warning: invalid use of incomplete type 'struct ev_loop'
/opt/local/include/ev.h:166: warning: forward declaration of 'struct ev_loop'
*/
    }

    void EventIo::addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventIoWatcher *watcher, int32_t revents)) {
        this->addWatcher(socketFd, callback, EV_READ);
    }

    void EventIo::addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventIoWatcher *watcher, int32_t revents), int32_t flags) {
        EventIoWatcher *watcher = (EventIoWatcher *) malloc(sizeof(EventIoWatcher));
        ev_io_init(watcher, callback, socketFd, flags);
        ev_io_start(this->ioLoop, watcher);
        this->ioWatcherPool->insert(EventIoWatcherMap::value_type(socketFd, watcher));
    }

    EventIoWatcher *EventIo::getWatcher(int32_t socketFd) {
        EventIoWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->end()) {
            return it->second;
        } else {
            return NULL;
        }
    }

    void EventIo::removeWatcher(int32_t socketFd) {
        EventIoWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->end()) {
            // found the watcher in the pool
            ev_io_stop(this->ioLoop, it->second);
            this->ioWatcherPool->erase(it);
        }
    }

    void EventIo::clearWatchers() {
        if (this->ioWatcherPool->size() > 0) {
            EventIoWatcherIterator it;
            for (it = this->ioWatcherPool->begin(); it != this->ioWatcherPool->end(); /* no auto increment*/) {
                EventIoWatcher *watcher = it->second;
                ev_io_stop(this->ioLoop, watcher);
                this->ioWatcherPool->erase(it++);
            }
            this->ioWatcherPool->clear();
        }
    }

} /* namespace XCF */
