#ifndef XCF_THREAD_H_
#define XCF_THREAD_H_

#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "../../Declare.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

namespace XCF {

    typedef void *(ThreadStartFunc)(void *);

    class Thread {
        public:
            Thread();
            virtual ~Thread();
            /**
             * Create a new thread, and return the thread id.
             * The pointer of the threadId already initialized.
             */
            static inline pthread_t *createThread(pthread_t *threadId, ThreadStartFunc start) {
                pthread_create(threadId, NULL, start, NULL);
                return threadId;
            };
            /**
             * Create a new thread, and return the thread id.
             * A new pointer of the threadId would be created.
             */
            static inline pthread_t *createThread(ThreadStartFunc start) {
                pthread_t *threadId = (pthread_t *) malloc(sizeof(pthread_t));
                return Thread::createThread(threadId, start);
            };
            /**
             * Create a thread mutex, and return the mutex id.
             */
            static inline pthread_mutex_t *createLock() {
                pthread_mutex_t *lock = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
                if (pthread_mutex_init(lock, NULL) < 0) {
                    LogFactory::error(Utility::stringFormat("[Thread] error in init thread mutex: [%d] %s", errno, strerror(errno)));
                    return NULL;
                } else {
                    return lock;
                }
            };
            /**
             * Destory a thread mutex.
             */
            static inline int32_t destoryLock(pthread_mutex_t *lock) {
                if (pthread_mutex_destroy(lock) < 0) {
                    LogFactory::error(Utility::stringFormat("[Thread] error in destory thread mutex: [%d] %s", errno, strerror(errno)));
                    return INVALID_RESULT;
                } else {
                    delete lock;
                    return VALID_RESULT;
                }
            };
        protected:
            pthread_t *threadId;
    };

} /* namespace XCF */

#endif /* XCF_THREAD_H_ */
