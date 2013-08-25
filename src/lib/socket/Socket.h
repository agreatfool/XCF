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

    namespace SocketProtocol {
        enum SocketProtocol {
            TCP = IPPROTO_TCP,
            UDP = IPPROTO_UDP
        };
    }

    namespace SocketStatus {
        enum SocketStatus {
            Closed,
            Opened,
            Connected,
            Error
        };
    }

    class Socket {
        public:
            Socket(std::string host, uint16_t port);
            Socket(std::string host, uint16_t port, uint16_t type);
            virtual ~Socket();
        protected:
            // Inputed
            std::string        socketHost;
            uint16_t           socketPort;
            // Type
            uint16_t           socketType;
            // Status
            int16_t            socketStatus;
            // Generated
            Log*               logger;
            int32_t            socketFd;
            struct sockaddr_in socketAddr;
            // Functions
            int32_t init();
            void release();
    };

} /* namespace XCF */

#endif /* XCF_SOCKET_H_ */
