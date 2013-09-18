#include "EventPeriodic.h"
#include "../../log/Log.h"
#include "../../utility/Utility.h"

DEF_NS_XCF_BEGIN

EventPeriodic::EventPeriodic(): Event(), timerWatcherPool(new EventPeriodicWatcherMap()) {
    LogFactory::get()->info("[EventPeriodic] EventPeriodic loop Initialized ...");
}

EventPeriodic::~EventPeriodic() {
    delete this->timerWatcherPool;
}

void EventPeriodic::addWatcher(std::string name, EventPeriodicCallback callback, double interval) {
    EventPeriodicWatcher *watcher = (EventPeriodicWatcher *) malloc(sizeof(EventPeriodicWatcher));
    ev_periodic_init(watcher, callback, 0., interval, 0);
    ev_periodic_start(this->loop, watcher);
    this->timerWatcherPool->add(name, watcher);
    LogFactory::get()->info("[EventPeriodic] Periodic event added with name: " + name);
}

void EventPeriodic::removeWatcher(std::string name) {
    EventPeriodicWatcherIterator it = this->findWatcher(name);
    if (it != this->timerWatcherPool->getMap()->end()) {
        // found the watcher in the pool
        ev_periodic_stop(this->loop, it->second);
        this->timerWatcherPool->getMap()->erase(it);
    }
    LogFactory::get()->info("[EventPeriodic] Periodic event removed with name: %s" + name);
}

void EventPeriodic::clearWatchers() {
    if (!this->timerWatcherPool->empty()) {
        EventPeriodicWatcherIterator it;
        for (it = this->timerWatcherPool->getMap()->begin(); it != this->timerWatcherPool->getMap()->end(); /* no auto increment*/) {
            EventPeriodicWatcher *watcher = it->second;
            ev_periodic_stop(this->loop, watcher);
            this->timerWatcherPool->getMap()->erase(it++);
        }
        this->timerWatcherPool->getMap()->clear();
    }
    LogFactory::get()->info("[EventPeriodic] Periodic events cleared ...");
}

DEF_NS_XCF_END
