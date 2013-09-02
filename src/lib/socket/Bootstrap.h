#ifndef XCF_BOOTSTRAP_H_
#define XCF_BOOTSTRAP_H_

#include "../../Declare.h"
#include "Socket.h"
#include "../log/Log.h"
#include "../event/Event.h"

namespace XCF {

    class Bootstrap {
        public:
            Bootstrap(uint16_t protocolType);
            virtual ~Bootstrap();
            virtual int32_t start() = 0;
            virtual int32_t stop() = 0;
            static inline EventIo *getEventIo() {
                return Bootstrap::eventIo;
            };
            static inline SocketPool *getSocketPool() {
                return Bootstrap::socketPool;
            };
            static inline Log *getLogger() {
                return Bootstrap::logger;
            };
        protected:
            uint16_t          socketProtocolType;
            static EventIo    *eventIo;
            static SocketPool *socketPool;
            static Log        *logger;
    };

    class ServerBootstrap: public Bootstrap {
        public:
            ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port);
            virtual ~ServerBootstrap();
            int32_t start();
            int32_t stop();
            static void acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents);
            static void readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents);
        protected:
            Socket *serverSocket;
    };

    class ClientBootstrap: public Bootstrap {
        public:
            ClientBootstrap(uint16_t protocolType);
            virtual ~ClientBootstrap();
            int32_t start();
            int32_t stop();
        protected:
    };

} /* namespace XCF */

#endif /* XCF_BOOTSTRAP_H_ */
