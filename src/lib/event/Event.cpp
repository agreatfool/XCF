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
        LogFactory::get()->debug("[Event] ~Event ...");
    }

    void Event::startLoop() {
        ev_run(this->loop, 0);
        LogFactory::get()->debug("[Event] Loop started ...");
    }

    void Event::stopLoop() {
        this->suspendLoop();
        this->clearWatchers();
        ev_break(this->loop, EVBREAK_ALL);
        LogFactory::get()->debug("[Event] Loop stopped ...");
    }

    void Event::suspendLoop() {
        ev_suspend(this->loop);
    }

    void Event::resumeLoop() {
        ev_resume(this->loop);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventIo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo::EventIo(): Event(), ioWatcherPool(new EventIoWatcherMap()) {
        LogFactory::get()->debug("[EventIo] EventIo loop Initialized ...");
    }

    EventIo::~EventIo() {
        delete this->ioWatcherPool;
    }

    void EventIo::addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags) {
        EventIoWatcher *watcher = (EventIoWatcher *) malloc(sizeof(EventIoWatcher));
        ev_io_init(watcher, callback, socketFd, flags);
        ev_io_start(this->loop, watcher);
        this->ioWatcherPool->add(socketFd, watcher);
        LogFactory::get()->debug(Utility::stringFormat("[EventIo] Io event added with fd: %d", socketFd));
    }

    void EventIo::removeWatcher(int32_t socketFd) {
        EventIoWatcherIterator it = this->findWatcher(socketFd);
        if (it != this->ioWatcherPool->getMap()->end()) {
            // found the watcher in the pool
            ev_io_stop(this->loop, it->second);
            this->ioWatcherPool->getMap()->erase(it);
        }
        LogFactory::get()->debug(Utility::stringFormat("[EventIo] Io event removed with fd: %d", socketFd));
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
        LogFactory::get()->debug("[EventIo] Io events cleared ...");
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventPeriodic
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventPeriodic::EventPeriodic(): Event(), timerWatcherPool(new EventPeriodicWatcherMap()) {
        LogFactory::get()->debug("[EventPeriodic] EventPeriodic loop Initialized ...");
    }

    EventPeriodic::~EventPeriodic() {
        delete this->timerWatcherPool;
    }

    void EventPeriodic::addWatcher(std::string name, EventPeriodicCallback callback, double interval) {
        EventPeriodicWatcher *watcher = (EventPeriodicWatcher *) malloc(sizeof(EventPeriodicWatcher));
        ev_periodic_init(watcher, callback, 0., interval, 0);
        ev_periodic_start(this->loop, watcher);
        this->timerWatcherPool->add(name, watcher);
        LogFactory::get()->debug("[EventPeriodic] Periodic event added with name: " + name);
    }

    void EventPeriodic::removeWatcher(std::string name) {
        EventPeriodicWatcherIterator it = this->findWatcher(name);
        if (it != this->timerWatcherPool->getMap()->end()) {
            // found the watcher in the pool
            ev_periodic_stop(this->loop, it->second);
            this->timerWatcherPool->getMap()->erase(it);
        }
        LogFactory::get()->debug("[EventPeriodic] Periodic event removed with name: %s" + name);
    }

    void EventPeriodic::clearWatchers() {
        if (this->timerWatcherPool->count() > 0) {
            EventPeriodicWatcherIterator it;
            for (it = this->timerWatcherPool->getMap()->begin(); it != this->timerWatcherPool->getMap()->end(); /* no auto increment*/) {
                EventPeriodicWatcher *watcher = it->second;
                ev_periodic_stop(this->loop, watcher);
                this->timerWatcherPool->getMap()->erase(it++);
            }
            this->timerWatcherPool->getMap()->clear();
        }
        LogFactory::get()->debug("[EventPeriodic] Periodic events cleared ...");
    }

} /* namespace XCF */
