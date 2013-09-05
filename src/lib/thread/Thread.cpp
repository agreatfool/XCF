#include "Thread.h"

namespace XCF {

    Thread::Thread(): threadId((pthread_t *) malloc(sizeof(pthread_t))) {
    }

    Thread::~Thread() {
    }

} /* namespace XCF */
