#ifndef XCF_THREAD_H_
#define XCF_THREAD_H_

#include <pthread.h>

namespace XCF {

    class Thread {
        public:
            Thread();
            virtual ~Thread();
    };

} /* namespace XCF */

#endif /* XCF_THREAD_H_ */
