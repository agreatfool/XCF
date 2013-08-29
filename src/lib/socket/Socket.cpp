#include "Socket.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketBuffer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SocketBuffer::SocketBuffer():
        buffer(new char[SOCK_BUFFER_LENGTH]()) {}

    SocketBuffer::SocketBuffer(char buff[]):
        buffer(new char[SOCK_BUFFER_LENGTH]())
    {
        this->copyBuffer(buff);
    }

    SocketBuffer::~SocketBuffer() {
        delete []this->buffer;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Socket
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Socket::Socket(
            std::string host, uint16_t port,
            uint16_t protocol, uint16_t endType
        ):
        socketHost(host), socketPort(port),
        socketProtocolType(protocol), socketEndType(endType),
        socketStatus(SocketStatus::CLOSED),
        logger(LogFactory::get()), socketFd(INIT_SOCKET_FD)
    {
        this->socketInit();
    }

    Socket::Socket(
            int32_t socketFd, struct sockaddr_in socketAddr, uint16_t protocol
        ):
        socketHost(""), socketPort(0),
        socketProtocolType(protocol), socketEndType(SocketEndType::CLIENT),
        socketStatus(SocketStatus::CONNECTED),
        logger(LogFactory::get()), socketFd(socketFd), socketAddr(socketAddr)
    {
        this->socketHost = inet_ntoa(socketAddr.sin_addr);
        this->socketPort = ntohs(socketAddr.sin_port);
    }

    Socket::~Socket() {
        this->socketRelease();
        delete this->logger;
    }

    int32_t Socket::socketInit() {
        if (this->socketStatus <= SocketStatus::CLOSED) {
            // init socket fd
            this->socketFd = socket(PF_INET, SOCK_STREAM, 0);

            if (this->socketFd < 0) {
                this->socketRelease();
                this->logger->error(Utility::stringFormat("[Socket] socketFd initialization failed: [%d] %s", errno, strerror(errno)));
                return INVALID_RESULT;
            }

            // set socket options
            if (this->setNonBlock() < 0 || this->setKeepAlive() < 0 || this->setReuseAddr() < 0) {
                this->socketRelease();
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
                    this->socketRelease();
                    this->logger->error(Utility::stringFormat("[Socket] socket binding failed: [%d] %s", errno, strerror(errno)));
                    return INVALID_RESULT;
                }
                if (listen(this->socketFd, SOCK_LISTEN_BACKLOG) < 0) {
                    this->socketRelease();
                    this->logger->error(Utility::stringFormat("[Socket] socket listen failed: [%d] %s", errno, strerror(errno)));
                    return INVALID_RESULT;
                }
            } else if (this->socketEndType == SocketEndType::CLIENT) {
                // client: connect
                if (connect(this->socketFd, (struct sockaddr *) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                    this->socketRelease();
                    this->logger->error(Utility::stringFormat("[Socket] socket connect failed: [%d] %s", errno, strerror(errno)));
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

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketPool
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SocketPool::SocketPool(): socketPool(new SocketPoolMap()) {}

    SocketPool::~SocketPool() {
        this->clearSockets();
        delete this->socketPool;
    }

    void SocketPool::addSocket(Socket *socket) {
        this->socketPool->insert(SocketPoolMap::value_type(socket->getSocketFd(), socket));
    }

    void SocketPool::removeSocket(int32_t socketFd) {
        SocketPoolIterator it = this->findSocket(socketFd);
        if (it != this->socketPool->end()) {
            // found the socket in the pool
            delete it->second;
            this->socketPool->erase(it);
        }
    }

    Socket *SocketPool::getSocket(int32_t socketFd) {
        SocketPoolIterator it = this->findSocket(socketFd);
        if (it != this->socketPool->end()) {
            return it->second;
        } else {
            return NULL;
        }
    }

    void SocketPool::clearSockets() {
        if (this->socketPool->size() > 0) {
            SocketPoolIterator it;
            for (it = this->socketPool->begin(); it != this->socketPool->end(); /* no auto increment*/) {
                delete it->second;
                this->socketPool->erase(it++);
            }
            this->socketPool->clear();
        }
    }

} /* namespace XCF */
