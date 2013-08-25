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
    #define INVALID_RESULT    -1

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
            NONE,       // class constructed
            INITED,     // class socket initialized
            CLOSED,     // socket closed
            Opened,     // socket opened (server: bind listen)
            Connected,  // socket connected (client: connect)
            Error       // socket error
        };
    }

    class Socket {
        public:
            Socket(std::string host, uint16_t port, uint16_t protocol, uint16_t endType);
            virtual ~Socket();
            // Server
            int32_t listen();
            // Client
            int32_t connect();
        protected:
            // Inputed
            std::string        socketHost;
            uint16_t           socketPort;
            uint16_t           socketProtocolType;
            uint16_t           socketEndType;
            // Status
            int16_t            socketStatus;
            // Generated
            Log*               logger;
            int32_t            socketFd;
            struct sockaddr_in socketAddr;
            // Functions
            void init();
    };

} /* namespace XCF */

#endif /* XCF_SOCKET_H_ */
