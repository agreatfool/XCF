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
        protected:
            uint16_t   socketProtocolType;
            static Log *logger;
    };

    class ServerBootstrap: public Bootstrap {
        public:
            ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port);
            virtual ~ServerBootstrap();
            // FIXME Is it a good idea to expose EventIo & SocketPool to public static ?
            // Is there any better way to let callback function to visit the eventIo & socketPool ?
            static EventIo    *eventIo;
            static SocketPool *socketPool;
            int32_t start();
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
            static EventIo    *eventIo;
            static SocketPool *socketPool;
        protected:
    };

} /* namespace XCF */

#endif /* XCF_BOOTSTRAP_H_ */
