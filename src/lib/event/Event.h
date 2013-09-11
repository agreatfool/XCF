#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include "../../Common.h"
#include "../model/Map.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- Event
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class Event {
    public:
        Event();
        virtual ~Event();
        inline EventLoop *getEventLoop();
        /**
         * Start the event loop.
         */
        void startLoop();
        /**
         * Stop all the event loop.
         * If any active watchers in the event loop,
         * they would all be terminated.
         */
        void stopLoop();
        /**
         * Suspend the event loop.
         */
        void suspendLoop();
        /**
         * Resume the event loop.
         */
        void resumeLoop();
        /**
         * Remove all the EventWatcher in the EventWatcherMap.
         */
        virtual void clearWatchers() = 0;
    protected:
        EventLoop  *loop;
        ThreadLock *lock;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- EventIo
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef Map<int32_t, EventIoWatcher>                  EventIoWatcherMap;
typedef std::map<int32_t, EventIoWatcher *>::iterator EventIoWatcherIterator;
typedef void (*EventIoCallback)(EventLoop *loop, EventIoWatcher *watcher, int32_t revents);

class EventIo: public Event {
    public:
        EventIo();
        virtual ~EventIo();
        /**
         * malloc & ev_io_init & ev_io_start a EventIoWatcher.
         * And add it into the EventIoWatcherMap.
         */
        void addWatcher(int32_t socketFd, EventIoCallback callback);
        void addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags);
        /**
         * Get EventWatcher from the EventIoWatcherMap.
         * If specified EventIoWatcher not found, NULL pointer returned.
         */
        EventIoWatcher *getWatcher(int32_t socketFd) const;
        /**
         * Remove the EventIoWatcher, if it exists in the EventIoWatcherMap.
         */
        void removeWatcher(int32_t socketFd);
        /**
         * Remove all the EventIoWatcher in the EventIoWatcherMap.
         */
        void clearWatchers();
        /**
         * Get EventIoWatcherMap size.
         */
        int32_t getWatchersCount() const;
        /**
         * Find the EventIoWatcher in the EventIoWatcherMap.
         * EventIoWatcherIterator returned.
         */
        EventIoWatcherIterator findWatcher(int32_t socketFd) const;
    protected:
        EventIoWatcherMap *ioWatcherPool;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- EventPeriodic
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef Map<std::string, EventPeriodicWatcher>                  EventPeriodicWatcherMap;
typedef std::map<std::string, EventPeriodicWatcher *>::iterator EventPeriodicWatcherIterator;
typedef void (*EventPeriodicCallback)(EventLoop *loop, EventPeriodicWatcher *watcher, int32_t revents);

class EventPeriodic: public Event {
    public:
        EventPeriodic();
        virtual ~EventPeriodic();
        /**
         * malloc & ev_periodic_init & ev_periodic_start a EventPeriodicWatcher.
         * And add it into the EventPeriodicWatcherMap.
         */
        void addWatcher(std::string name, EventPeriodicCallback callback, double interval);
        /**
         * Get EventWatcher from the EventPeriodicWatcherMap.
         * If specified EventPeriodicWatcher not found, NULL pointer returned.
         */
        EventPeriodicWatcher *getWatcher(std::string name) const;
        /**
         * Remove the EventPeriodicWatcher, if it exists in the EventPeriodicWatcherMap.
         */
        void removeWatcher(std::string name);
        /**
         * Remove all the EventPeriodicWatcher in the EventPeriodicWatcherMap.
         */
        void clearWatchers();
        /**
         * Get EventPeriodicWatcher size.
         */
        int32_t getWatchersCount() const;
        /**
         * Find the EventPeriodicWatcher in the EventPeriodicWatcherMap.
         * EventPeriodicWatcherIterator returned.
         */
        EventPeriodicWatcherIterator findWatcher(std::string name) const;
    protected:
        EventPeriodicWatcherMap *timerWatcherPool;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- inline Implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- Event
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline EventLoop *Event::getEventLoop() {
    return this->loop;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- EventIo
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline void EventIo::addWatcher(int32_t socketFd, EventIoCallback callback) {
    this->addWatcher(socketFd, callback, EV_READ);
}
inline EventIoWatcher *EventIo::getWatcher(int32_t socketFd) const {
    return this->ioWatcherPool->get(socketFd);
}
inline int32_t EventIo::getWatchersCount() const {
    return this->ioWatcherPool->count();
}
inline EventIoWatcherIterator EventIo::findWatcher(int32_t socketFd) const {
    return this->ioWatcherPool->find(socketFd);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- EventPeriodic
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline EventPeriodicWatcher *EventPeriodic::getWatcher(std::string name) const {
    return this->timerWatcherPool->get(name);
}
inline int32_t EventPeriodic::getWatchersCount() const {
    return this->timerWatcherPool->count();
}
inline EventPeriodicWatcherIterator EventPeriodic::findWatcher(std::string name) const {
    return this->timerWatcherPool->find(name);
}

DEF_NS_XCF_END

#endif /* XCF_EVENT_H_ */
