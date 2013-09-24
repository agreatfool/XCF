#include "ThreadPool.h"
#include "Thread.h"
#include "ThreadUtil.h"
#include "../log/Log.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* ThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
ThreadPool::ThreadPool():
    threadPool(new Vector<Thread>()), lock(ThreadUtil::createLock()) {}

ThreadPool::~ThreadPool() {
    this->threadPool->clear();
    delete this->threadPool;
    delete this->lock;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* DynamicThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
DynamicThreadPool::DynamicThreadPool(): ThreadPool() {};

DynamicThreadPool::~DynamicThreadPool() {};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* IdleThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
IdleThreadPool::IdleThreadPool():
    DynamicThreadPool(),
    prevPos(-1),
    idleQueue(new Queue<Thread>()) {}

IdleThreadPool::~IdleThreadPool() {
    this->idleQueue->clear(false); // do not delete "Thread *", since they are not maintained in this queue
    delete this->idleQueue;
    this->threadPool->clear();
    delete this->threadPool;
    delete this->lock;
}

Thread *IdleThreadPool::next() {
    Thread *targetThread;

    ThreadUtil::lock(this->lock);

    if (this->threadPool->empty()) {
        LogFactory::get()->warn("[ThreadPool] Thread pool is empty, NULL pointer returned in next().");
        targetThread = NULL;
    } else {
        uint64_t targetPos = this->prevPos + 1;
        if (targetPos >= this->threadPool->count()) {
            targetPos = 0; // return to the first
        }
        targetThread = this->threadPool->get(targetPos);
        this->prevPos = targetPos;
    }

    ThreadUtil::lock(this->lock);

    return targetThread;
}

Thread *IdleThreadPool::idle() {
    Thread *targetThread;
    if (!this->idleQueue->empty()) {
        targetThread = this->idleQueue->pop();
    } else {
        targetThread = this->next();
    }
    return targetThread;
}

DEF_NS_XCF_END
