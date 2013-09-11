#ifndef XCF_SOCKETPIPLINE_H_
#define XCF_SOCKETPIPLINE_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class SocketPipline {
    public:
        SocketPipline();
        virtual ~SocketPipline();
};

class SocketPiplineFactory {
    public:
        SocketPiplineFactory();
        virtual ~SocketPiplineFactory();
        SocketPipline *getPipline();
};

DEF_NS_XCF_END

#endif /* XCF_SOCKETPIPLINE_H_ */
