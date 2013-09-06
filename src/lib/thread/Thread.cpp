#include "Thread.h"

namespace XCF {

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
        threadId(NULL), lock(NULL), cond(NULL), status(ThreadStatus::INITED)
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

    void Thread::stop() {
        ThreadUtil::lock(this->lock);

        this->status = ThreadStatus::STOPPED;
        ThreadUtil::signalCond(this->cond);

        ThreadUtil::unlock(this->lock);
        ThreadUtil::joinThread(this->threadId);

        LogFactory::get()->info("Thread stopped.");
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ThreadUtil
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    pthread_t *ThreadUtil::createThread(ThreadStartFunc start, Thread *thread) {
        pthread_t *threadId;
        pthread_create(threadId, NULL, start, (void *)thread);
        return threadId;
    };

    pthread_mutex_t *ThreadUtil::createLock() {
        pthread_mutex_t *lock;
        if (pthread_mutex_init(lock, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_init: [%d] %s", errno, strerror(errno)));
            return NULL;
        } else {
            return lock;
        }
    };

    int32_t ThreadUtil::destroyLock(pthread_mutex_t *lock) {
        if (pthread_mutex_destroy(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_destroy: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            delete lock;
            return VALID_RESULT;
        }
    };

    int32_t ThreadUtil::lock(pthread_mutex_t *lock) {
        if (pthread_mutex_lock(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_lock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    };

    int32_t ThreadUtil::unlock(pthread_mutex_t *lock) {
        if (pthread_mutex_unlock(lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_unlock: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    };

    pthread_cond_t *ThreadUtil::createCond() {
        pthread_cond_t *cond;
        if (pthread_cond_init(cond, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_inits: [%d] %s", errno, strerror(errno)));
            return NULL;
        } else {
            return cond;
        }
    };

    int32_t ThreadUtil::destroyCond(pthread_cond_t *cond) {
        if (pthread_cond_destroy(cond) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_destroy: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            delete cond;
            return VALID_RESULT;
        }
    };

    int32_t ThreadUtil::waitCond(pthread_cond_t *cond, pthread_mutex_t *lock) {
        if (pthread_cond_wait(cond, lock) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_wait: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    };

    int32_t ThreadUtil::signalCond(pthread_cond_t *cond) {
        if (pthread_cond_signal(cond) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_signal: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    };

    int32_t ThreadUtil::joinThread(pthread_t *threadId) {
        if (pthread_join(*threadId, NULL) < 0) {
            LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_join: [%d] %s", errno, strerror(errno)));
            return INVALID_RESULT;
        } else {
            return VALID_RESULT;
        }
    };

    pthread_t ThreadUtil::getThreadId() {
        return pthread_self();
    };

} /* namespace XCF */
