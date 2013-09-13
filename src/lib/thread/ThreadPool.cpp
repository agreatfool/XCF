#include "ThreadPool.h"
#include "../log/Log.h"

DEF_NS_XCF_BEGIN

ThreadPool::ThreadPool():
    prevPos(-1),
    threadPool(new Vector<Thread>()),
    idleQueue(new Queue<Thread>()),
    lock(ThreadUtil::createLock()) {}

ThreadPool::~ThreadPool() {
    this->idleQueue->clear(false); // do not delete "Thread *", since they are not maintained in this queue
    delete this->idleQueue;
    this->threadPool->clear();
    delete this->threadPool;
    delete this->lock;
}

void ThreadPool::addThread(Thread *thread) {
    this->threadPool->push(thread);
    this->idleQueue->push(thread);
}

void ThreadPool::markIdleThread(Thread *thread) {
    if (this->threadPool->found(thread)) { // target thread is exactly managed by this pool
        this->idleQueue->push(thread);
    }
}

Thread *ThreadPool::next() {
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

Thread *ThreadPool::idle() {
    Thread *targetThread;
    if (!this->idleQueue->empty()) {
        targetThread = this->idleQueue->pop();
    } else {
        targetThread = this->next();
    }
    return targetThread;
}

DEF_NS_XCF_END
