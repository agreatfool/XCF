#include "EventIo.h"
#include "../../log/Log.h"
#include "../../utility/Utility.h"

DEF_NS_XCF_BEGIN

EventIo::EventIo(): Event(), ioWatcherPool(new EventIoWatcherMap()) {
    LogFactory::get()->info("[EventIo] EventIo loop Initialized ...");
}

EventIo::~EventIo() {
    delete this->ioWatcherPool;
}

std::string EventIo::getEventName() { return "EventIo"; }

void EventIo::addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags) {
    EventIoWatcher *watcher = (EventIoWatcher *) malloc(sizeof(EventIoWatcher));
    ev_io_init(watcher, callback, socketFd, flags);
    ev_io_start(this->loop, watcher);
    this->ioWatcherPool->add(socketFd, watcher);
    LogFactory::get()->info(Utility::stringFormat("[%s] Io event added with fd: %d", this->getEventName().c_str(), socketFd));
}

void EventIo::removeWatcher(int32_t socketFd) {
    EventIoWatcherIterator it = this->findWatcher(socketFd);
    if (it != this->ioWatcherPool->getMap()->end()) {
        // found the watcher in the pool
        ev_io_stop(this->loop, it->second);
        this->ioWatcherPool->getMap()->erase(it);
    }
    LogFactory::get()->info(Utility::stringFormat("[%s] Io event removed with fd: %d", this->getEventName().c_str(), socketFd));
}

void EventIo::clearWatchers() {
    if (this->ioWatcherPool->empty()) {
        EventIoWatcherIterator it;
        for (it = this->ioWatcherPool->getMap()->begin(); it != this->ioWatcherPool->getMap()->end(); /* no auto increment*/) {
            EventIoWatcher *watcher = it->second;
            ev_io_stop(this->loop, watcher);
            this->ioWatcherPool->getMap()->erase(it++);
        }
        this->ioWatcherPool->getMap()->clear();
    }
    LogFactory::get()->info(Utility::stringFormat("[%s] Io events cleared ...", this->getEventName().c_str()));
}

DEF_NS_XCF_END
