#include "Event.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- Event
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Event::Event():
        loop(
#if defined (__APPLE__)
            ev_loop_new(EVBACKEND_KQUEUE | EVFLAG_NOENV)
#elif defined (__linux)
            ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV)
#endif
        ) {};

    Event::~Event() {
        this->stopLoop();
        ev_loop_destroy(this->loop);
    };

    void Event::stopLoop() {
        this->suspendLoop();
        this->clearWatchers();
        ev_break(this->loop, EVBREAK_ALL);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventIo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo::EventIo(): Event(), ioWatcherPool(new EventIoWatcherMap()) {}

    EventIo::~EventIo() {
        delete this->ioWatcherPool;
//        delete this->loop;
// FIXME should we delete the "this->loop" ?
/*
In compiling:
../src/lib/event/Event.cpp:27: warning: possible problem detected in invocation of delete operator:
../src/lib/event/Event.cpp:27: warning: invalid use of incomplete type 'struct ev_loop'
/opt/local/include/ev.h:166: warning: forward declaration of 'struct ev_loop'
*/
    }

    void EventIo::addWatcher(int32_t socketFd, EventCallback callback, int32_t flags) {
        EventIoWatcher *watcher = (EventIoWatcher *) malloc(sizeof(EventIoWatcher));
        ev_io_init(watcher, callback, socketFd, flags);
        ev_io_start(this->loop, watcher);
        this->ioWatcherPool->add(socketFd, watcher);
    }

    void EventIo::removeWatcher(int32_t socketFd) {
        EventIoWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->getMap()->end()) {
            // found the watcher in the pool
            ev_io_stop(this->loop, it->second);
            this->ioWatcherPool->getMap()->erase(it);
        }
    }

    void EventIo::clearWatchers() {
        if (this->ioWatcherPool->count() > 0) {
            EventIoWatcherIterator it;
            for (it = this->ioWatcherPool->getMap()->begin(); it != this->ioWatcherPool->getMap()->end(); /* no auto increment*/) {
                EventIoWatcher *watcher = it->second;
                ev_io_stop(this->loop, watcher);
                this->ioWatcherPool->getMap()->erase(it++);
            }
            this->ioWatcherPool->getMap()->clear();
        }
    }

} /* namespace XCF */
