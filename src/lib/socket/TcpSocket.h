#ifndef XCF_TCPSOCKET_H_
#define XCF_TCPSOCKET_H_

#include "Socket.h"

namespace XCF {

    class TcpSocket: public Socket {
        public:
            TcpSocket(std::string host, uint16_t port);
            virtual ~TcpSocket();
            int32_t createServer(); // TODO listen 绑定事件 开始循环 | 如何处理客户端连接？
            int32_t createClient(); // TODO connect 绑定事件 开始循环
        protected:
            virtual void eventHandler(struct ev_loop *loop, struct ev_io *watcher, int revents) = 0;
    };

} /* namespace XCF */

#endif /* XCF_TCPSOCKET_H_ */
