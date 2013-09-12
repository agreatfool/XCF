#ifndef XCF_EVENTPERIODIC_H_
#define XCF_EVENTPERIODIC_H_

#include "../../../Common.h"
#include "../Event.h"

DEF_NS_XCF_BEGIN

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

#endif /* XCF_EVENTPERIODIC_H_ */
