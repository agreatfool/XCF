#ifndef XCF_EVENTIO_H_
#define XCF_EVENTIO_H_

#include "../../../Common.h"
#include "../Event.h"

DEF_NS_XCF_BEGIN

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
//- inline Implementations
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

DEF_NS_XCF_END

#endif /* XCF_EVENTIO_H_ */
