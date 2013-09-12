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

/**
 * Thread work flow:
 * 1. DerivedThread *derived = new DerivedThread();
 *     at this time, the thread class is an empty class, has no functionality, it's not ready.
 * 2. derived->init();
 *     initialize the Thread class, create pthread lock & pthread condition & pthread thread.
 * 3. threadStartFunc();
 *     when creating pthread thread, threadStartFunc will be called.
 * 4. derived->prepareToRun();
 *     let derived class to do some self init logics.
 * 5. derived->run();
 *     thread start to run, basically it's an endless loop to check thread status & process logics.
 */
class Thread {
    public:
        Thread();
        virtual ~Thread();
        /**
         * Get thread id in "Thread".
         */
        ThreadId *getThreadId();
        /**
         * Get numeric thread id.
         */
        uint64_t getNumericThreadId();
        /**
         * Initialize the thread.
         * Create pthread lock & pthread condition & create pthread thread.
         */
        void init();
        /**
         * Preparation before the thread start to run.
         */
        virtual int32_t prepareToRun() = 0;
        /**
         * Start to run the thread.
         * Basically, this function is an endless loop to check thread status & process logic.
         * It can be overwrote by derived class implementation.
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
         * Get thread status.
         */
        uint16_t getStatus();
        /**
         * Check whether thread is sleeping.
         */
        bool isSleeping();
    protected:
        ThreadId   *threadId;
        uint64_t   numericThreadId;
        ThreadLock *lock;
        ThreadCond *cond;
        uint16_t   status;
        char       retVal[64];
        /**
         * Whether the thread can be blocked at present.
         * Check whether the thread has no work to do, if true block it to sleep.
         */
        virtual bool canBeBlocked() = 0;
        /**
         * Check thread status.
         * If thread status marked STOPPED, pthread_exit it.
         * If thread this->canBeBlocked(), set it to BLOCKED, and pthread_cond_wait.
         * Otherwise mark status RUNNING.
         */
        void checkThreadStatus();
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
inline uint64_t Thread::getNumericThreadId() {
    if (this->numericThreadId == 0) {
#if defined (__APPLE__)
        pthread_threadid_np(NULL, &this->numericThreadId);
#elif defined (__linux)
        this->numericThreadId = gettid();
#endif
    }
    return this->numericThreadId;
}
inline uint16_t Thread::getStatus() {
    return this->status;
}

DEF_NS_XCF_END

#endif /* XCF_THREAD_H_ */
