#include "Socket.h"

namespace XCF {

    Socket::Socket(
            std::string host, uint16_t port,
            uint16_t protocol, uint16_t endType
        ):
        socketHost(host), socketPort(port),
        socketProtocolType(protocol), socketEndType(endType),
        socketStatus(SocketStatus::CLOSED),
        logger(LogFactory::get()), socketFd(INIT_SOCKET_FD) {}

    Socket::Socket(
            int32_t socketFd, struct sockaddr_in socketAddr, uint16_t protocol
        ):
        socketHost(""), socketPort(0),
        socketProtocolType(protocol), socketEndType(SocketEndType::CLIENT),
        socketStatus(SocketStatus::CONNECTED),
        logger(LogFactory::get()), socketFd(socketFd), socketAddr(socketAddr) {}

    Socket::~Socket() {
        this->socketRelease();
        delete this->logger;
    }

    int32_t Socket::socketInit() {
        if (this->socketStatus <= SocketStatus::CLOSED) {
            // init socket fd
            this->socketFd = socket(PF_INET, SOCK_STREAM, 0);

            if (this->socketFd < 0) {
                this->socketFd = INVALID_SOCKET_FD;
                this->logger->error("[Socket] socketFd initialization failed!");
                return INVALID_RESULT;
            }

            // build address
            bzero(&this->socketAddr, sizeof(this->socketAddr));
            this->socketAddr.sin_family = AF_INET;
            this->socketAddr.sin_port = htons(this->socketPort);
            this->socketAddr.sin_addr.s_addr = inet_addr(this->socketHost.c_str());

            this->socketStatus = SocketStatus::OPENED;

            if (this->socketEndType == SocketEndType::SERVER) {
                // server: bind & listen
                if (bind(this->socketFd, (struct sockaddr *) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                    this->socketFd = INVALID_SOCKET_FD;
                    this->logger->error("[Socket] socket binding failed!");
                    return INVALID_RESULT;
                }
                if (listen(this->socketFd, SOCK_LISTEN_BACKLOG) < 0) {
                    this->socketFd = INVALID_SOCKET_FD;
                    this->logger->error("[Socket] listen failed!");
                    return INVALID_RESULT;
                }
            } else if (this->socketEndType == SocketEndType::CLIENT) {
                // client: connect
                if (connect(this->socketFd, (struct sockaddr *) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                    this->socketFd = INVALID_SOCKET_FD;
                    this->logger->error("[Socket] connect failed!");
                    return INVALID_RESULT;
                }
            }

            this->socketStatus = SocketStatus::CONNECTED;
        }

        return VALID_RESULT;
    }

    int32_t Socket::socketRelease() {
        if (this->socketFd > 0) {
            close(this->socketFd);
        }
        this->socketStatus = SocketStatus::CLOSED;
        this->socketFd = INVALID_SOCKET_FD;
        bzero(&this->socketAddr, sizeof(this->socketAddr));

        return VALID_RESULT;
    }

} /* namespace XCF */
