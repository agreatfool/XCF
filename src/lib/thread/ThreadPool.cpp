#include "ThreadPool.h"

DEF_NS_XCF_BEGIN

ThreadPool::ThreadPool():
    prevPos(-1), pool(new Vector()), lock(ThreadUtil::createLock()) {}

ThreadPool::~ThreadPool() {
    this->pool->clear();
    delete this->pool;
    delete this->lock;
}

void ThreadPool::addThread(Thread *thread) {
    this->pool->push(thread);
}

void ThreadPool::removeThread(uint64_t threadId) {
    this->pool->remove(threadId);
}

Thread *ThreadPool::next() {
    Thread *targetThread;

    ThreadUtil::lock(this->lock);

    if (this->pool->count() == 0) {
        targetThread = NULL;
    } else {
        uint64_t targetPos = this->prevPos + 1;
        if (targetPos >= this->pool->count()) {
            targetPos = 0; // return to the first
        }
        targetThread = this->pool->get(targetPos);
    }

    ThreadUtil::lock(this->lock);

    return targetThread;
}

DEF_NS_XCF_END
