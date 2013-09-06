#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

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
             * malloc & ev_io_init & ev_io_start a EventWatcher.
             * And add it into the EventWatcherMap.
             */
            inline void addWatcher(int32_t socketFd, EventIoCallback callback) {
                this->addWatcher(socketFd, callback, EV_READ);
            };
            void addWatcher(int32_t socketFd, EventIoCallback callback, int32_t flags);
            /**
             * Get EventWatcher from the EventWatcherMap.
             * If specified EventWatcher not found, NULL pointer returned.
             */
            inline EventIoWatcher *getWatcher(int32_t socketFd) const {
                return this->ioWatcherPool->get(socketFd);
            };
            /**
             * Remove the EventWatcher, if it exists in the EventWatcherMap.
             */
            void removeWatcher(int32_t socketFd);
            /**
             * Remove all the EventWatcher in the EventWatcherMap.
             */
            void clearWatchers();
            /**
             * Get EventWatcherMap size.
             */
            inline int32_t getWatchersCount() const {
                return this->ioWatcherPool->count();
            };
            /**
             * Find the EventWatcher in the EventWatcherMap.
             * EventWatcherIterator returned.
             */
            inline EventIoWatcherIterator findWatcher(int32_t socketFd) const {
                return this->ioWatcherPool->find(socketFd);
            };
        protected:
            EventIoWatcherMap *ioWatcherPool;
    };

} /* namespace XCF */

#endif /* XCF_EVENT_H_ */
