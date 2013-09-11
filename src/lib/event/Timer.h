#ifndef XCF_TIMER_H_
#define XCF_TIMER_H_

#include "../../Common.h"
#include "../event/Event.h"
#include "../thread/Thread.h"

DEF_NS_XCF_BEGIN

class TimerThread: public Thread, public EventPeriodic {
    public:
        TimerThread();
        virtual ~TimerThread();
        virtual void run();
    protected:
       virtual bool canBeBlocked();
       virtual void process();
};

class Timer {
    public:
        virtual ~Timer();
        static TimerThread *get();
    protected:
        Timer();
        static TimerThread *instance;
        static ThreadLock  lock;
        // Stop the compiler generating methods of copy the object
        Timer(Timer const& copy);            // Not Implemented
        Timer& operator=(Timer const& copy); // Not Implemented
};

DEF_NS_XCF_END

#endif /* XCF_TIMER_H_ */
