#include "Thread.h"
#include "ThreadUtil.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* Thread
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
void *threadStartFunc(void *argv) {
    if (!argv) {
        return 0;
    }
    Thread *thread = (Thread *)argv;

    if (thread->prepareToRun() < 0) {
        return 0;
    } else {
        thread->run();
        return 0;
    }
}

Thread::Thread():
    threadId(NULL), numericThreadId(0), lock(NULL), cond(NULL), status(ThreadStatus::INITED), retVal()
{
    /*
     * Since "ThreadUtil::createThread(threadStartFunc, this);" would call function "threadStartFunc",
     * and at this time, the derived class has not been initialized, the instance of the object is "Thread",
     * the param "void *argv" of "threadStartFunc" is given as the instance of "Thread".
     * Logic of "thread->init()" will report error "pure virtual method called" (this function of Thread is still pure virtual function).
     * So the initialization logics have to be moved into funtion "Thread::init()".
     */
}

Thread::~Thread() {
    delete this->threadId;
    ThreadUtil::destroyLock(this->lock);
    ThreadUtil::destroyCond(this->cond);
}

void Thread::init() {
    this->lock = ThreadUtil::createLock();
    this->cond = ThreadUtil::createCond();
    this->threadId = ThreadUtil::createThread(threadStartFunc, this);
    LogFactory::get()->info(Utility::stringFormat("[%s] Thread initialized ...", this->getThreadName().c_str()));
}

void Thread::run() {
    LogFactory::get()->info(Utility::stringFormat("[%d] Thread %d start to run ...", this->getThreadName().c_str(), this->getNumericThreadId()));
    while (1) {
        this->checkThreadStatus();
        this->process();
    }
}

void Thread::wakeup() {
    ThreadUtil::lock(this->lock);

    if (this->status == ThreadStatus::BLOCKED && !this->canBeBlocked() ) {
        LogFactory::get()->info(Utility::stringFormat("[%s] Thread %d wakeup ...", this->getThreadName().c_str(), this->getNumericThreadId()));
        ThreadUtil::signalCond(this->cond);
    }

    ThreadUtil::unlock(this->lock);
}

void Thread::stop() {
    ThreadUtil::lock(this->lock);

    this->status = ThreadStatus::STOPPED;
    ThreadUtil::signalCond(this->cond);

    ThreadUtil::unlock(this->lock);
    ThreadUtil::joinThread(this->threadId);

    LogFactory::get()->info(Utility::stringFormat("[%s] Thread %d stopped ...", this->getThreadName().c_str(), this->getNumericThreadId()));
}

void Thread::checkThreadStatus() {
    ThreadUtil::lock(this->lock);

    while (this->canBeBlocked() || this->status == ThreadStatus::STOPPED) {
        if (this->status == ThreadStatus::STOPPED) {
            LogFactory::get()->info(Utility::stringFormat("[%s] Thread %d marked as stopped, exit ...", this->getThreadName().c_str(), this->getNumericThreadId()));
            pthread_exit((void *) this->retVal);
        }
        this->status = ThreadStatus::BLOCKED;
        LogFactory::get()->info(Utility::stringFormat("[%s] Thread %d can be blocked, sleeping ...", this->getThreadName().c_str(), this->getNumericThreadId()));
        ThreadUtil::waitCond(this->cond, this->lock);
    }

    this->status = ThreadStatus::RUNNING;

    ThreadUtil::unlock(this->lock);
}

bool Thread::isSleeping() {
    if (this->status == ThreadStatus::BLOCKED) {
        return true;
    } else {
        return false;
    }
}

DEF_NS_XCF_END
