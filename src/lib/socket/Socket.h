#ifndef XCF_SOCKET_H_
#define XCF_SOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <ev.h>

#include "../log/Log.h"

namespace XCF {

    #define INIT_SOCKET_FD     0
    #define INVALID_SOCKET_FD -1
    #define VALID_RESULT       0
    #define INVALID_RESULT    -1

    #define LISTEN_BACKLOG     1024
    #define MSG_BUFFER_LENGTH  1024 * 16

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
            inline int32_t  getSocketFd() {
                return this->socketFd;
            };
            inline uint16_t getSocketStatus() {
                return this->socketStatus;
            };
            int32_t socketInit();
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
