#ifndef XCF_SOCKETEVENT_H_
#define XCF_SOCKETEVENT_H_

#include <ev.h>

namespace XCF {

    class SocketEvent {
        public:
            SocketEvent();
            virtual ~SocketEvent();
            virtual void eventHandle(struct ev_loop *loop, struct ev_io *watcher, int revents) = 0;
    };

} /* namespace XCF */

#endif /* XCF_SOCKETEVENT_H_ */
