#ifndef XCF_THREADPOOL_H_
#define XCF_THREADPOOL_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class ThreadPool {
    public:
        ThreadPool();
        virtual ~ThreadPool();
        void addThread(Thread *thread);
        void markIdleThread(Thread *thread);
        /**
         * Walk to next worker thread, according to "this->prevPos", and return it.
         */
        Thread *next();
        /**
         * Check idle thread queue, provide idle first.
         * If queue empty, use "this->next()" to get worker by pos.
         */
        Thread *idle();
    protected:
        int64_t prevPos; // init value is -1, so cannot use uint64_t
        Vector<Thread> *threadPool;
        Queue<Thread> *idleQueue;
        ThreadLock *lock;
};

DEF_NS_XCF_END

#endif /* XCF_THREADPOOL_H_ */
