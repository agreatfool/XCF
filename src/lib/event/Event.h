#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include <iostream>
#include <ev.h>

#include "../model/Map.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- Event
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    typedef struct ev_loop EventLoop;

    class Event {
        public:
            Event();
            virtual ~Event();
            inline EventLoop *getEventLoop() {
                return this->loop;
            };
            /**
             * Start the event loop.
             */
            inline int32_t startLoop() {
                return ev_run(this->loop, 0);
            };
            /**
             * Stop all the event loop.
             * If any active watchers in the event loop,
             * they would all be terminated.
             */
            void stopLoop();
            /**
             * Suspend the event loop.
             */
            inline void suspendLoop() {
                ev_suspend(this->loop);
            };
            /**
             * Resume the event loop.
             */
            inline void resumeLoop() {
                ev_resume(this->loop);
            };
            /**
             * Remove all the EventWatcher in the EventWatcherMap.
             */
            virtual void clearWatchers() = 0;
        protected:
            EventLoop *loop;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventIo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    typedef struct ev_io EventIoWatcher;
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
            inline void addWatcher(int32_t socketFd, EventIoCallback callback) {
                this->addWatcher(socketFd, callback, EV_READ);
            };
            void addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags);
            /**
             * Get EventWatcher from the EventIoWatcherMap.
             * If specified EventIoWatcher not found, NULL pointer returned.
             */
            inline EventIoWatcher *getWatcher(int32_t socketFd) const {
                return this->ioWatcherPool->get(socketFd);
            };
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
            inline int32_t getWatchersCount() const {
                return this->ioWatcherPool->count();
            };
            /**
             * Find the EventIoWatcher in the EventIoWatcherMap.
             * EventIoWatcherIterator returned.
             */
            inline EventIoWatcherIterator findWatcher(int32_t socketFd) const {
                return this->ioWatcherPool->find(socketFd);
            };
        protected:
            EventIoWatcherMap *ioWatcherPool;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- EventTimer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    // FIXME 完成 timer event loop & 使用timer来定期输出日志
    // 完成 Thread::wakeup 和 Thread::checkBlockedOrStopped, 弄清楚 Thread::canBeBlocked 到底应该怎么返回
    typedef struct ev_timer EventTimerWatcher;
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

} /* namespace XCF */

#endif /* XCF_EVENT_H_ */
