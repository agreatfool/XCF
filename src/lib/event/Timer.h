#ifndef XCF_TIMER_H_
#define XCF_TIMER_H_

#include "../../XCF.h"

namespace XCF {

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

} /* namespace XCF */

#endif /* XCF_TIMER_H_ */
