#ifndef XCF_TIMER_H_
#define XCF_TIMER_H_

#include "../../XCF.h"
#include "../event/Event.h"
#include "../thread/Thread.h"

namespace XCF {

    class Timer {
        public:
            virtual ~Timer();
            static EventPeriodic *get();
        protected:
            Timer();
            static EventPeriodic *instance;
            static ThreadLock    lock;
            // Stop the compiler generating methods of copy the object
            Timer(Timer const& copy);            // Not Implemented
            Timer& operator=(Timer const& copy); // Not Implemented
    };

} /* namespace XCF */

#endif /* XCF_TIMER_H_ */
