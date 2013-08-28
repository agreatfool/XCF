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
