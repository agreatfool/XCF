#ifndef XCF_THREADPOOL_H_
#define XCF_THREADPOOL_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class Thread;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* ThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class ThreadPool {
    public:
        ThreadPool();
        virtual ~ThreadPool();
    protected:
        Vector<Thread> *threadPool;
        ThreadLock *lock;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* FixedThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
template <typename THREAD_TYPE>
class FixedThreadPool: public ThreadPool {
    public:
        FixedThreadPool(uint16_t size): ThreadPool() {
            for (uint16_t i = 0; i < size; ++i) {
                THREAD_TYPE *thread = new THREAD_TYPE();
                thread->init();
                thread->run();
                this->threadPool->push(thread);
            }
        };
        virtual ~FixedThreadPool() {};
        inline THREAD_TYPE *getThread(uint16_t id) {
            return this->threadPool->get(id);
        };
        inline THREAD_TYPE *getThreadViaShard(uint16_t shardId) {
            return this->threadPool->get(shardId % this->threadPool->count());
        };
    protected:
        Vector<THREAD_TYPE> *threadPool;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* DynamicThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class DynamicThreadPool: public ThreadPool {
    public:
        DynamicThreadPool();
        virtual ~DynamicThreadPool();
        void addThread(Thread *thread);
        Thread *getThread(uint16_t id);
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* IdleThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class IdleThreadPool: public DynamicThreadPool {
    public:
        IdleThreadPool();
        virtual ~IdleThreadPool();
        void markIdleThread(Thread *thread);
        /**
         * Walk to next worker thread, according to "this->prevPos", and return it.
         */
        Thread *next();
        /**
         * Check idle thread queue, provide idle thread first.
         * If queue empty, use "this->next()" to get worker by pos.
         */
        Thread *idle();
    protected:
        int64_t prevPos; // init value is -1, so cannot use uint64_t
        Queue<Thread> *idleQueue;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* inline implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* DynamicThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline void DynamicThreadPool::addThread(Thread *thread) {
    this->threadPool->push(thread);
}
inline Thread *DynamicThreadPool::getThread(uint16_t id) {
    return this->threadPool->get(id);
}
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* IdleThreadPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline void IdleThreadPool::markIdleThread(Thread *thread) {
    if (this->threadPool->found(thread)) { // target thread is exactly managed by this pool
        this->idleQueue->push(thread);
    }
}

DEF_NS_XCF_END

#endif /* XCF_THREADPOOL_H_ */
