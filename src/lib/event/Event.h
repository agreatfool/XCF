#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include <map>

#include <ev.h>

namespace XCF {

    typedef struct ev_loop EventLoop;
    typedef struct ev_io   EventWatcher;

    typedef std::map<int32_t, EventWatcher *>           EventWatcherMap;
    typedef std::map<int32_t, EventWatcher *>::iterator EventWatcherIterator;

    class EventIo {
        public:
            EventIo();
            virtual ~EventIo();
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
             * malloc & ev_io_init & ev_io_start a EventWatcher.
             * And add it into the EventWatcherMap.
             */
            void addWatcher(int32_t socketFd, void (*callback)(EventLoop *loop, EventWatcher *watcher, int revents));
            /**
             * Get EventWatcher from the EventWatcherMap.
             * If specified EventWatcher not found, NULL pointer returned.
             */
            EventWatcher *getWatcher(int32_t socketFd);
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
            inline EventWatcherIterator findWatcher(int32_t socketFd) {
                return this->ioWatcherPool->find(socketFd);
            };
        protected:
            EventLoop       *ioLoop;
            EventWatcherMap *ioWatcherPool;
    };

} /* namespace XCF */

#endif /* XCF_EVENT_H_ */
