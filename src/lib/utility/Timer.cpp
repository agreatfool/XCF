#include "Timer.h"

namespace XCF {

    EventPeriodic *Timer::instance = NULL;
    ThreadLock Timer::lock = PTHREAD_MUTEX_INITIALIZER;

    Timer::~Timer() {}

    EventPeriodic *Timer::get() {
        if (Timer::instance == NULL) {
            ThreadUtil::lock(Timer::lock);
            Timer::instance = new EventPeriodic();
            ThreadUtil::unlock(Timer::lock);
        }
        return Timer::instance;
    }

} /* namespace XCF */
