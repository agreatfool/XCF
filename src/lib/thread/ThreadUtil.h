#ifndef XCF_THREADUTIL_H_
#define XCF_THREADUTIL_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class Thread;

class ThreadUtil {
    public:
        ThreadUtil();
        virtual ~ThreadUtil();
        /**
         * Create a new thread, and return the thread id.
         * A new pointer of the threadId would be created.
         */
        static ThreadId *createThread(ThreadStartFunc start, Thread *thread);
        /**
         * Create a thread mutex, and return the mutex id.
         */
        static ThreadLock *createLock();
        /**
         * Destroy a thread mutex.
         */
        static int32_t destroyLock(ThreadLock *lock);
        /**
         * Lock a thread mutex.
         */
        static int32_t lock(ThreadLock *lock);
        /**
         * Unlock a thread mutex.
         */
        static int32_t unlock(ThreadLock *lock);
        /**
         * Create thread condition.
         */
        static ThreadCond *createCond();
        /**
         * Destroy thread condition.
         */
        static int32_t destroyCond(ThreadCond *cond);
        /**
         * Wait on a condition variable.
         */
        static int32_t waitCond(ThreadCond *cond, ThreadLock *lock);
        /**
         * Unblock a thread waiting for a condition variable.
         */
        static int32_t signalCond(ThreadCond *cond);
        /**
         * Join a thread.
         * Suspends the calling thread to wait for successful termination of the thread specified.
         */
        static int32_t joinThread(ThreadId *threadId);
        /**
         * Get the thread id via "pthread_self()".
         * The return value is reference not a pointer!
         */
        static ThreadId getThreadId();
};

DEF_NS_XCF_END

#endif /* XCF_THREADUTIL_H_ */
