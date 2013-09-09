#ifndef XCF_THREAD_H_
#define XCF_THREAD_H_

#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "../../XCF.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

namespace XCF {

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
            virtual int32_t init() = 0;
            /**
             * Run the thread. Thread logic goes here.
             */
            virtual void run() = 0;
            /**
             * Whether current can be blocked. Check whether the thread has no work to do, if true block it to sleep.
             */
            virtual bool canBeBlocked() = 0;
            /**
             * Wakeup the thread.
             */
            void wakeup();
            /**
             * Stop the thread.
             */
            void stop();
            /**
             * Check whether thread status is marked as BLOCKED or STOPPED.
             * If true, process according logics.
             */
            void checkBlockedOrStopped();
        protected:
            ThreadId   *threadId;
            ThreadLock *lock;
            ThreadCond *cond;
            uint16_t   status;
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

} /* namespace XCF */

#endif /* XCF_THREAD_H_ */
