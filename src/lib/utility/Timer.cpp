#include "Timer.h"

namespace XCF {

    EventPeriodic *Timer::instance = NULL;

    Timer::~Timer() {}

    EventPeriodic *Timer::get() {
        if (Timer::instance == NULL) {
            Timer::instance = new EventPeriodic();
        }
        return Timer::instance;
    }

} /* namespace XCF */