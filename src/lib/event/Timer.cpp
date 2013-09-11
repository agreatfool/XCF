#include "Timer.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* TimerThread
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
TimerThread::TimerThread(): Thread(), EventPeriodic() {}

TimerThread::~TimerThread() {}

void TimerThread::run() {
    /*
     * Since this thread is used by libev event loop,
     * the event loop would block the thread and check event,
     * there is no real logic in this thread itself.
     * So we overwrite the method "run()", to start the loop in it,
     * and dismiss the the process function.
     */
    this->startLoop();
}

bool TimerThread::canBeBlocked() { return false; }

int32_t TimerThread::prepareToRun() { return XCF_VALID_RESULT; }

void TimerThread::process() { /* no logic to process here */ }

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* Timer
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
TimerThread *Timer::instance = NULL;
ThreadLock Timer::lock = PTHREAD_MUTEX_INITIALIZER;

Timer::~Timer() {}

TimerThread *Timer::get() {
    if (Utility::isNullPtr(Timer::instance)) {
        ThreadUtil::lock(&Timer::lock);
        Timer::instance = new TimerThread();
        Timer::instance->init();
        ThreadUtil::unlock(&Timer::lock);
    }
    return Timer::instance;
}

DEF_NS_XCF_END
