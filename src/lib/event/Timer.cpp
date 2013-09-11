#include "Timer.h"
#include "../event/Event.h"
#include "../thread/Thread.h"

DEF_NS_XCF_BEGIN

    EventPeriodic *Timer::instance = NULL;

    Timer::~Timer() {}

    EventPeriodic *Timer::get() {
        if (Timer::instance == NULL) {
            Timer::instance = new EventPeriodic();
        }
        return Timer::instance;
    }

DEF_NS_XCF_END
