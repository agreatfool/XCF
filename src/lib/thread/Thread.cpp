#include "Thread.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Thread
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    void *threadStartFunc(void *argv) {
        if (!argv) {
            return 0;
        }
        Thread *thread = (Thread *)argv;

        if (thread->init() < 0) {
            return 0;
        } else {
            thread->run();
            return 0;
        }
    }

    Thread::Thread():
        threadId(NULL), lock(NULL), cond(NULL), status(ThreadStatus::INITED), retVal()
    {
        this->threadId = ThreadUtil::createThread(threadStartFunc, this);
        this->lock = ThreadUtil::createLock();
        this->cond = ThreadUtil::createCond();
    }

    Thread::~Thread() {
        delete this->threadId;
        ThreadUtil::destroyLock(this->lock);
        ThreadUtil::destroyCond(this->cond);
    }

    void Thread::wakeup() {
        ThreadUtil::lock(this->lock);

        if (this->status == ThreadStatus::BLOCKED && !this->canBeBlocked() ) {
            ThreadUtil::signalCond(this->cond);
        }

        ThreadUtil::unlock(this->lock);
    }

    void Thread::stop() {
        ThreadUtil::lock(this->lock);

        this->status = ThreadStatus::STOPPED;
        ThreadUtil::signalCond(this->cond);

        ThreadUtil::unlock(this->lock);
        ThreadUtil::joinThread(this->threadId);

        LogFactory::get()->info("Thread stopped.");
    }

    void Thread::checkThreadStatus() {
        ThreadUtil::lock(this->lock);

        while (this->canBeBlocked() || this->status == ThreadStatus::STOPPED) {
            if (this->status == ThreadStatus::STOPPED) {
                pthread_exit((void *) this->retVal);
            }
            this->status = ThreadStatus::BLOCKED;
            ThreadUtil::waitCond(this->cond, this->lock);
        }

        this->status = ThreadStatus::RUNNING;

        ThreadUtil::unlock(this->lock);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ThreadUtil
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ThreadId *ThreadUtil::createThread(ThreadStartFunc start, Thread *thread) {
        ThreadId *threadId = (ThreadId *) malloc(sizeof(ThreadId));
        pthread_create(threadId, NULL, start, (void *)thread);
        return threadId;
    }

    ThreadLock *ThreadUtil::createLock() {
        ThreadLock *lock = (ThreadLock *) malloc(sizeof(ThreadLock));
        if (pthread_mutex_init(lock, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_init: [%d] %s", errno, strerror(errno)));
            return NULL;
        } else {
            return lock;
        }
    }

    int32_t ThreadUtil::destroyLock(ThreadLock *lock) {
        if (pthread_mutex_destroy(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_destroy: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            delete lock;
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::lock(ThreadLock *lock) {
        if (pthread_mutex_lock(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_lock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::lock(ThreadLock lock) {
        if (pthread_mutex_lock(&lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_lock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::unlock(ThreadLock *lock) {
        if (pthread_mutex_unlock(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_unlock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::unlock(ThreadLock lock) {
        if (pthread_mutex_unlock(&lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_unlock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    ThreadCond *ThreadUtil::createCond() {
        ThreadCond *cond = (ThreadCond *) malloc(sizeof(ThreadCond));
        if (pthread_cond_init(cond, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_inits: [%d] %s", errno, strerror(errno)));
            return NULL;
        } else {
            return cond;
        }
    }

    int32_t ThreadUtil::destroyCond(ThreadCond *cond) {
        if (pthread_cond_destroy(cond) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_destroy: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            delete cond;
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::waitCond(ThreadCond *cond, ThreadLock *lock) {
        if (pthread_cond_wait(cond, lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_wait: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::signalCond(ThreadCond *cond) {
        if (pthread_cond_signal(cond) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_signal: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    int32_t ThreadUtil::joinThread(ThreadId *threadId) {
        if (pthread_join(*threadId, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_join: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    }

    ThreadId ThreadUtil::getThreadId() {
        return pthread_self();
    };

DEF_NS_XCF_END
