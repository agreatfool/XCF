#ifndef XCF_THREAD_H_
#define XCF_THREAD_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

typedef void *(ThreadStartFunc)(void *); // argument (void *) actually is (Thread *)

namespace ThreadStatus {
    enum ThreadStatus {
        INITED,
        BLOCKED,
        RUNNING,
        STOPPED
    };
}

class Thread {
    public:
        Thread();
        virtual ~Thread();
        /**
         * Get thread id in "Thread".
         */
        ThreadId *getThreadId();
        /**
         * Initialize the thread.
         */
        virtual int32_t init();
        /**
         * Start to run the thread.
         * Basically, this function is an endless loop to check thread status & process logic.
         * It can be overwrote by child class implementation.
         */
        virtual void run();
        /**
         * Wakeup the thread.
         */
        void wakeup();
        /**
         * Stop the thread.
         */
        void stop();
        /**
         * Check thread status.
         * If thread status marked STOPPED, pthread_exit it.
         * If thread this->canBeBlocked(), set it to BLOCKED, and pthread_cond_wait.
         * Otherwise mark status RUNNING.
         */
        void checkThreadStatus();
    protected:
        ThreadId   *threadId;
        ThreadLock *lock;
        ThreadCond *cond;
        uint16_t   status;
        char       retVal[64];
        /**
         * Whether current can be blocked.
         * Check whether the thread has no work to do, if true block it to sleep.
         */
        virtual bool canBeBlocked() = 0;
        /**
         * Real thread logic goes here.
         */
        virtual void process() = 0;
};

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
        static int32_t lock(ThreadLock lock);
        /**
         * Unlock a thread mutex.
         */
        static int32_t unlock(ThreadLock *lock);
        static int32_t unlock(ThreadLock lock);
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

class ThreadPool {

};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- inline Implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- Thread
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline ThreadId *Thread::getThreadId() {
    return this->threadId;
}

DEF_NS_XCF_END

#endif /* XCF_THREAD_H_ */
