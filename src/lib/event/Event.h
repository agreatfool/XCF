#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include <iostream>
#include <ev.h>

#include "../model/Map.h"
#include "../../XCF.h"

namespace XCF {

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
            int32_t startLoop();
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
    //- EventTimer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    // FIXME 完成 timer event loop & 使用timer来定期输出日志
    // 完成 Thread::wakeup 和 Thread::checkBlockedOrStopped, 弄清楚 Thread::canBeBlocked 到底应该怎么返回
    typedef Map<std::string, EventTimerWatcher>                  EventTimerWatcherMap;
    typedef std::map<std::string, EventTimerWatcher *>::iterator EventTimerWatcherIterator;
    typedef void (*EventTimerCallback)(EventLoop *loop, EventTimerWatcher *watcher, int32_t revents);

    class EventTimer: public Event {
        EventTimer();
        virtual ~EventTimer();
        /**
         * malloc & ev_timer_init & ev_timer_start a EventTimerWatcher.
         * And add it into the EventTimerWatcherMap.
         */
        inline void addWatcher(std::string name, EventIoCallback callback) {
            this->addWatcher(name, callback, EV_READ);
        };
        void addWatcher(std::string name, EventIoCallback callback, int32_t flags);
        /**
         * Get EventWatcher from the EventTimerWatcherMap.
         * If specified EventTimerWatcher not found, NULL pointer returned.
         */
        inline EventTimerWatcher *getWatcher(std::string name) const {
            return this->timerWatcherPool->get(name);
        };
        /**
         * Remove the EventTimerWatcher, if it exists in the EventTimerWatcherMap.
         */
        void removeWatcher(std::string name);
        /**
         * Remove all the EventTimerWatcher in the EventTimerWatcherMap.
         */
        void clearWatchers();
        /**
         * Get EventTimerWatcher size.
         */
        inline int32_t getWatchersCount() const {
            return this->timerWatcherPool->count();
        };
        /**
         * Find the EventTimerWatcher in the EventTimerWatcherMap.
         * EventTimerWatcherIterator returned.
         */
        inline EventTimerWatcherIterator findWatcher(std::string name) const {
            return this->timerWatcherPool->find(name);
        };
    protected:
        EventTimerWatcherMap *timerWatcherPool;
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

} /* namespace XCF */

#endif /* XCF_EVENT_H_ */
