#ifndef XCF_TIMER_H_
#define XCF_TIMER_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

    class EventPeriodic;

    class Timer {
        public:
            virtual ~Timer();
            static EventPeriodic *get();
        protected:
            Timer();
            static EventPeriodic *instance;
            // Stop the compiler generating methods of copy the object
            Timer(Timer const& copy);            // Not Implemented
            Timer& operator=(Timer const& copy); // Not Implemented
    };

DEF_NS_XCF_END

#endif /* XCF_TIMER_H_ */
