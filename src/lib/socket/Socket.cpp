#include "Socket.h"

namespace XCF {

    Socket::Socket(
            std::string host, uint16_t port,
            uint16_t protocol, uint16_t endType
        ):
        socketHost(host), socketPort(port),
        socketProtocolType(protocol), socketEndType(endType),
        socketStatus(SocketStatus::NONE),
        logger(LogFactory::get()), socketFd(INIT_SOCKET_FD)
    {
        this->init();
    }

    Socket::~Socket() {
        if (this->socketFd > 0) {
            close(this->socketFd);
        }
        delete this->logger;
    }

    void Socket::init() {
        if (this->socketStatus < SocketStatus::INITED) {
            // not initialized yet
            this->socketFd = socket(PF_INET, SOCK_STREAM, 0);

            if (this->socketFd > 0) {
                bzero(&this->socketAddr, sizeof(this->socketAddr));

                this->socketAddr.sin_family = AF_INET;
                this->socketAddr.sin_port = htons(this->socketPort);
                this->socketAddr.sin_addr.s_addr = inet_addr(this->socketHost.c_str());

                if (bind(this->socketFd, (struct sockaddr*) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                    this->socketFd = INVALID_SOCKET_FD;
                    this->logger->error("[Socket] socket binding failed!");
                } else {
                    this->socketStatus = SocketStatus::INITED;
                }
            } else {
                this->socketFd = INVALID_SOCKET_FD;
                this->logger->error("[Socket] socketFd initialization failed!");
            }
        }
    }

    int32_t Socket::listen() {
        if (this->socketEndType != SocketEndType::SERVER) {
            // invalid function call
            this->logger->error("[Socket] cannot call \"listen\" via client socket!");
            return INVALID_RESULT;
        } else if (this->socketStatus < SocketStatus::INITED) {
            // socket not correctly initialized
            return INVALID_RESULT;
        }
        return 1;
    }

    int32_t Socket::connect() {
        if (this->socketEndType != SocketEndType::CLIENT) {
            // invalid function call
            this->logger->error("[Socket] cannot call \"connect\" via server socket!");
            return INVALID_RESULT;
        } else if (this->socketStatus < SocketStatus::INITED) {
            // socket not correctly initialized
            return INVALID_RESULT;
        }
        return 1;
    }

} /* namespace XCF */
