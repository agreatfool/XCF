#ifndef XCF_EVENT_H_
#define XCF_EVENT_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

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
//- inline Implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline EventLoop *Event::getEventLoop() {
    return this->loop;
}

DEF_NS_XCF_END

#endif /* XCF_EVENT_H_ */
