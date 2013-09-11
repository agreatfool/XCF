#ifndef XCF_BOOTSTRAP_H_
#define XCF_BOOTSTRAP_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class EventIo;
class Socket;
class SocketPool;

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
        static ServerBootstrap *init(uint16_t protocolType, std::string host, uint16_t port);
        static ServerBootstrap *get();
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
        static ClientBootstrap *init(uint16_t protocolType);
        static ClientBootstrap *get();
        int32_t start();
        int32_t stop();
    protected:
        static ClientBootstrap *instance;
        ClientBootstrap(uint16_t protocolType);
        // Stop the compiler generating methods of copy the object
        ClientBootstrap(ClientBootstrap const& copy);            // Not Implemented
        ClientBootstrap& operator=(ClientBootstrap const& copy); // Not Implemented
};

DEF_NS_XCF_END

#endif /* XCF_BOOTSTRAP_H_ */
