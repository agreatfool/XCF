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
            inline EventIo *getEventIo() {
                return this->eventIo;
            };
            inline SocketPool *getSocketPool() {
                return this->socketPool;
            };
            virtual int32_t start() = 0;
            virtual int32_t stop() = 0;
        protected:
            uint16_t   socketProtocolType;
            EventIo    *eventIo;
            SocketPool *socketPool;
    };

    class ServerBootstrap: public Bootstrap {
        public:
            virtual ~ServerBootstrap();
            static inline ServerBootstrap *init(uint16_t protocolType, std::string host, uint16_t port) {
                if (Utility::isNullPtr(ServerBootstrap::instance)) {
                    ServerBootstrap::instance = new ServerBootstrap(protocolType, host, port);
                }
                return ServerBootstrap::instance;
            };
            static inline ServerBootstrap *get() {
                return ServerBootstrap::instance;
            };
            int32_t start();
            int32_t stop();
            static void acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents);
            static void readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents);
        protected:
            static ServerBootstrap *instance;
            ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port);
            // Stop the compiler generating methods of copy the object
            ServerBootstrap(ServerBootstrap const& copy);            // Not Implemented
            ServerBootstrap& operator=(ServerBootstrap const& copy); // Not Implemented
            Socket *serverSocket;
    };

    class ClientBootstrap: public Bootstrap {
        public:
            virtual ~ClientBootstrap();
            static inline ClientBootstrap *init(uint16_t protocolType) {
                if (Utility::isNullPtr(ClientBootstrap::instance)) {
                    ClientBootstrap::instance = new ClientBootstrap(protocolType);
                }
                return ClientBootstrap::instance;
            };
            static inline ClientBootstrap *get() {
                return ClientBootstrap::instance;
            };
            int32_t start();
            int32_t stop();
        protected:
            static ClientBootstrap *instance;
            ClientBootstrap(uint16_t protocolType);
            // Stop the compiler generating methods of copy the object
            ClientBootstrap(ClientBootstrap const& copy);            // Not Implemented
            ClientBootstrap& operator=(ClientBootstrap const& copy); // Not Implemented
    };

} /* namespace XCF */

#endif /* XCF_BOOTSTRAP_H_ */
