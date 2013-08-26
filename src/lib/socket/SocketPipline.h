#ifndef XCF_SOCKETPIPLINE_H_
#define XCF_SOCKETPIPLINE_H_

namespace XCF {

    class SocketPipline {
        public:
            SocketPipline();
            virtual ~SocketPipline();
    };

    class SocketPiplineFactory {
        public:
            SocketPiplineFactory();
            virtual ~SocketPiplineFactory();
            SocketPipline* getPipline();
    };

} /* namespace XCF */

#endif /* XCF_SOCKETPIPLINE_H_ */
