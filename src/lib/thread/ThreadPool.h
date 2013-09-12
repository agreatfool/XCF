#ifndef XCF_THREADPOOL_H_
#define XCF_THREADPOOL_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class ThreadPool {
    public:
        ThreadPool();
        virtual ~ThreadPool();
        void addThread(Thread *thread);
        void removeThread(uint64_t threadId); // threadId: pos in the vector
        Thread *next(); // walk to next worker thread, return it
    protected:
        int64_t prevPos; // init value is -1, so cannot use uint64_t
        Vector<Thread> *pool;
        ThreadLock *lock;
};

DEF_NS_XCF_END

#endif /* XCF_THREADPOOL_H_ */
