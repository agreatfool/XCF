#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include <map>

#include <ev.h>

namespace XCF {

    typedef struct ev_loop EventLoop;
    typedef struct ev_io   EventIoWatcher;

    typedef std::map<int32_t, EventIoWatcher *>           EventIoWatcherMap;
    typedef std::map<int32_t, EventIoWatcher *>::iterator EventIoWatcherIterator;

    class Event {
        public:
            Event();
            virtual ~Event();
            inline EventLoop getEventLoop() {
                return this->ioLoop;
            };
            /**
             * Start the event loop.
             */
            inline int32_t startLoop() {
                return ev_run(this->ioLoop, 0);
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
                ev_suspend(this->ioLoop);
            };
            /**
             * Resume the event loop.
             */
            inline void resumeLoop() {
                ev_resume(this->ioLoop);
            };
            /**
             * Remove all the EventWatcher in the EventWatcherMap.
             */
            virtual void clearWatchers() = 0;
        protected:
            EventLoop *ioLoop;
    };

    class EventIo: public Event {
        public:
            EventIo();
            virtual ~EventIo();
            /**
             * malloc & ev_io_init & ev_io_start a EventWatcher.
             * And add it into the EventWatcherMap.
             */
            void addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventIoWatcher *watcher, int32_t revents));
            void addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventIoWatcher *watcher, int32_t revents), int32_t flags);
            /**
             * Get EventWatcher from the EventWatcherMap.
             * If specified EventWatcher not found, NULL pointer returned.
             */
            EventIoWatcher *getWatcher(int32_t socketFd);
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
            inline int32_t getWatchersCount() {
                return this->ioWatcherPool->size();
            };
            /**
             * Find the EventWatcher in the EventWatcherMap.
             * EventWatcherIterator returned.
             */
            inline EventIoWatcherIterator findWatcher(int32_t socketFd) {
                return this->ioWatcherPool->find(socketFd);
            };
        protected:
            EventIoWatcherMap *ioWatcherPool;
    };

} /* namespace XCF */

#endif /* XCF_EVENT_H_ */
