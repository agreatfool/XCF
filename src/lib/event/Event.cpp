#include "Event.h"
#include "../thread/Thread.h"

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
        ), lock(ThreadUtil::createLock()) {}

    Event::~Event() {
        this->stopLoop();
        ev_loop_destroy(this->loop);
        ThreadUtil::destroyLock(this->lock);
    }

    int32_t Event::startLoop() {
        ThreadUtil::lock(this->lock);
        int32_t result = ev_run(this->loop, 0);
        ThreadUtil::unlock(this->lock);
        return result;
    }

    void Event::stopLoop() {
        this->suspendLoop();
        this->clearWatchers();
        ThreadUtil::lock(this->lock);
        ev_break(this->loop, EVBREAK_ALL);
        ThreadUtil::unlock(this->lock);
    }

    void Event::suspendLoop() {
        ThreadUtil::lock(this->lock);
        ev_suspend(this->loop);
        ThreadUtil::unlock(this->lock);
    }

    void Event::resumeLoop() {
        ThreadUtil::lock(this->lock);
        ev_resume(this->loop);
        ThreadUtil::unlock(this->lock);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventIo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo::EventIo(): Event(), ioWatcherPool(new EventIoWatcherMap()) {}

    EventIo::~EventIo() {
        delete this->ioWatcherPool;
    }

    void EventIo::addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags) {
        ThreadUtil::lock(this->lock);
        EventIoWatcher *watcher;
        ev_io_init(watcher, callback, socketFd, flags);
        ev_io_start(this->loop, watcher);
        this->ioWatcherPool->add(socketFd, watcher);
        ThreadUtil::unlock(this->lock);
    }

    void EventIo::removeWatcher(int32_t socketFd) {
        ThreadUtil::lock(this->lock);
        EventIoWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->getMap()->end()) {
            // found the watcher in the pool
            ev_io_stop(this->loop, it->second);
            this->ioWatcherPool->getMap()->erase(it);
        }
        ThreadUtil::unlock(this->lock);
    }

    void EventIo::clearWatchers() {
        ThreadUtil::lock(this->lock);
        if (this->ioWatcherPool->count() > 0) {
            EventIoWatcherIterator it;
            for (it = this->ioWatcherPool->getMap()->begin(); it != this->ioWatcherPool->getMap()->end(); /* no auto increment*/) {
                EventIoWatcher *watcher = it->second;
                ev_io_stop(this->loop, watcher);
                this->ioWatcherPool->getMap()->erase(it++);
            }
            this->ioWatcherPool->getMap()->clear();
        }
        ThreadUtil::unlock(this->lock);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventPeriodic
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventPeriodic::EventPeriodic(): Event(), timerWatcherPool(new EventPeriodicWatcherMap()) {}

    EventPeriodic::~EventPeriodic() {
        delete this->timerWatcherPool;
    }

    void EventPeriodic::addWatcher(std::string name, EventPeriodicCallback callback, double interval) {
        ThreadUtil::lock(this->lock);
        EventPeriodicWatcher *watcher;
        ev_periodic_init(watcher, callback, 0., interval, 0);
        ev_periodic_start(this->loop, watcher);
        this->timerWatcherPool->add(name, watcher);
        ThreadUtil::unlock(this->lock);
    }

    void EventPeriodic::removeWatcher(std::string name) {
        ThreadUtil::lock(this->lock);
        EventPeriodicWatcherIterator it = this->findWatcher(name);
        if (it != this->timerWatcherPool->getMap()->end()) {
            // found the watcher in the pool
            ev_periodic_stop(this->loop, it->second);
            this->timerWatcherPool->getMap()->erase(it);
        }
        ThreadUtil::unlock(this->lock);
    }

    void EventPeriodic::clearWatchers() {
        ThreadUtil::lock(this->lock);
        if (this->timerWatcherPool->count() > 0) {
            EventPeriodicWatcherIterator it;
            for (it = this->timerWatcherPool->getMap()->begin(); it != this->timerWatcherPool->getMap()->end(); /* no auto increment*/) {
                EventPeriodicWatcher *watcher = it->second;
                ev_periodic_stop(this->loop, watcher);
                this->timerWatcherPool->getMap()->erase(it++);
            }
            this->timerWatcherPool->getMap()->clear();
        }
        ThreadUtil::unlock(this->lock);
    }

} /* namespace XCF */
