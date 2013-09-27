#include "ThreadUtil.h"
#include "Thread.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

ThreadId *ThreadUtil::createThread(ThreadStartFunc start, Thread *thread) {
    ThreadId *threadId = (ThreadId *) malloc(sizeof(ThreadId));
    if (pthread_create(threadId, NULL, start, (void *)thread) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_create: [%d] %s", errno, strerror(errno)));
        return NULL;
    } else {
        return threadId;
    }
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
        return XCF_ERR;
    } else {
        delete lock;
        return XCF_OK;
    }
}

int32_t ThreadUtil::lock(ThreadLock *lock) {
    if (pthread_mutex_lock(lock) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_lock: [%d] %s", errno, strerror(errno)));
        return XCF_ERR;
    } else {
        return XCF_OK;
    }
}

int32_t ThreadUtil::unlock(ThreadLock *lock) {
    if (pthread_mutex_unlock(lock) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_mutex_unlock: [%d] %s", errno, strerror(errno)));
        return XCF_ERR;
    } else {
        return XCF_OK;
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
        return XCF_ERR;
    } else {
        delete cond;
        return XCF_OK;
    }
}

int32_t ThreadUtil::waitCond(ThreadCond *cond, ThreadLock *lock) {
    if (pthread_cond_wait(cond, lock) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_wait: [%d] %s", errno, strerror(errno)));
        return XCF_ERR;
    } else {
        return XCF_OK;
    }
}

int32_t ThreadUtil::signalCond(ThreadCond *cond) {
    if (pthread_cond_signal(cond) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_cond_signal: [%d] %s", errno, strerror(errno)));
        return XCF_ERR;
    } else {
        return XCF_OK;
    }
}

int32_t ThreadUtil::joinThread(ThreadId *threadId) {
    if (pthread_join(*threadId, NULL) < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Thread] error in pthread_join: [%d] %s", errno, strerror(errno)));
        return XCF_ERR;
    } else {
        return XCF_OK;
    }
}

ThreadId ThreadUtil::getThreadId() {
    return pthread_self();
}

DEF_NS_XCF_END
