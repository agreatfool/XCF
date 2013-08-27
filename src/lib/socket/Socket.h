#ifndef XCF_SOCKET_H_
#define XCF_SOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <ev.h>

#include "../../Declare.h"
#include "SocketBuffer.h"
#include "../log/Log.h"

namespace XCF {

    namespace SocketProtocol {
        enum SocketProtocol {
            TCP = IPPROTO_TCP,
            UDP = IPPROTO_UDP
        };
    }

    namespace SocketEndType {
        enum SocketEndType {
            CLIENT,
            SERVER
        };
    }

    namespace SocketStatus {
        enum SocketStatus {
            CLOSED,     // socket closed, init status
            OPENED,     // socket opened, address built
            CONNECTED,  // socket connected, listen finished | connect finished
            ERROR       // socket error
        };
    }

    class Socket {
        public:
            /**
             * Socket built to bind & listen to a server address.
             * OR
             * Socket built to connect to a server.
             */
            Socket(std::string host, uint16_t port, uint16_t protocol, uint16_t endType);
            /**
             * Socket accepted at server end, already connected.
             */
            Socket(int32_t socketFd, struct sockaddr_in socketAddr, uint16_t protocol);
            virtual ~Socket();
            /**
             * Get this->socketFd.
             */
            inline int32_t  getSocketFd() const {
                return this->socketFd;
            };
            /**
             * Get this->socketStatus.
             */
            inline uint16_t getSocketStatus() const {
                return this->socketStatus;
            };
            /**
             * Initialize the socket.
             * Server: socket() -> bind() -> listen()
             * Client: socket() -> connect()
             */
            int32_t socketInit();
            /**
             * Release all the socket resources,
             * make it the same status as before "socketInit()" was called.
             */
            int32_t socketRelease();
            /**
             * Read bytes from socket.
             */
            inline int32_t socketRead(SocketBuffer *buffer, int32_t length) {
                return recv(this->socketFd, buffer->getBuffer(), length, 0);
            };
            /**
             * Write bytes into socket.
             */
            inline int32_t socketWrite(SocketBuffer *buffer) {
                return send(this->socketFd, buffer->getBuffer(), strlen(buffer->getBuffer()), 0);
            };
        protected:
            // Inputed
            std::string        socketHost;
            uint16_t           socketPort;
            uint16_t           socketProtocolType;
            uint16_t           socketEndType;
            // Status
            int16_t            socketStatus;
            // Generated
            Log                *logger;
            int32_t            socketFd;
            struct sockaddr_in socketAddr;
    };

} /* namespace XCF */

#endif /* XCF_SOCKET_H_ */
