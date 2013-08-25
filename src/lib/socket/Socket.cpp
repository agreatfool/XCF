#include "Socket.h"

namespace XCF {

    Socket::Socket(std::string host, uint16_t port):
        socketHost(host), socketPort(port),
        socketType(SocketProtocol::TCP),
        socketStatus(SocketStatus::Closed), logger(LogFactory::get()),
        socketFd(INIT_SOCKET_FD) {}

    Socket::Socket(std::string host, uint16_t port, uint16_t type):
        socketHost(host), socketPort(port),
        socketType(type),
        socketStatus(SocketStatus::Closed), logger(LogFactory::get()),
        socketFd(INIT_SOCKET_FD) {}

    Socket::~Socket() {
        this->release();
        delete this->logger;
    }

    int32_t Socket::init() {
        this->socketFd = socket(PF_INET, SOCK_STREAM, 0);

        if (this->socketFd > 0) {
            bzero(&this->socketAddr, sizeof(this->socketAddr));

            this->socketAddr.sin_family = AF_INET;
            this->socketAddr.sin_port = htons(this->socketPort);
            this->socketAddr.sin_addr.s_addr = inet_addr(this->socketHost.c_str());

            if (bind(this->socketFd, (struct sockaddr*) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                this->logger->error("[Socket] socket binding failed!");
                this->release();
            }
        } else {
            this->logger->error("[Socket] socketFd initialization failed!");
        }

        return this->socketFd;
    }

    void Socket::release() {
        if(this->socketFd > 0) {
            close(this->socketFd);
        }
        this->socketFd = INVALID_SOCKET_FD;
        this->socketStatus = SocketStatus::Closed;
    }

} /* namespace XCF */
