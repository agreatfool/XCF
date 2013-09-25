#include "Socket.h"
#include "../log/Log.h"
#include "../event/Event.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* SocketBuffer
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
SocketBuffer::SocketBuffer():
    buffer(new char[XCF_SOCK_BUFFER_LENGTH]()) {}

SocketBuffer::SocketBuffer(char buff[]):
    buffer(new char[XCF_SOCK_BUFFER_LENGTH]())
{
    this->copyBuffer(buff);
}

SocketBuffer::~SocketBuffer() {
    delete []this->buffer;
}

void SocketBuffer::copyBuffer(char buff[]) {
    Utility::appendCharArray(this->buffer, buff, XCF_SOCK_BUFFER_LENGTH - strlen(this->buffer) - 1);
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
    socketStatus(SocketStatus::CLOSED), socketFd(XCF_INIT_SOCKET_FD)
{
    this->socketInit();
}

Socket::Socket(
        int32_t socketFd, struct sockaddr_in socketAddr, uint16_t protocol
    ):
    socketHost(""), socketPort(0),
    socketProtocolType(protocol), socketEndType(SocketEndType::CLIENT),
    socketStatus(SocketStatus::CONNECTED), socketFd(socketFd), socketAddr(socketAddr)
{
    this->socketHost = inet_ntoa(socketAddr.sin_addr);
    this->socketPort = ntohs(socketAddr.sin_port);
}

Socket::~Socket() {
    this->socketRelease();
}

Socket *Socket::socketAccept(int32_t acceptFromFd, uint16_t protocol) {
    struct sockaddr_in socketAddr;
    socklen_t socketLen = sizeof(socketAddr);
    int32_t socketFd = accept(acceptFromFd, (struct sockaddr *) &socketAddr, &socketLen);

    if (socketFd < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] accept socket failed: [%d] %s", errno, strerror(errno)));
        return NULL;
    }

    return new Socket(socketFd, socketAddr, protocol);
}

int32_t Socket::socketInit() {
    if (this->socketStatus <= SocketStatus::CLOSED) {
        // init socket fd
        this->socketFd = socket(PF_INET, SOCK_STREAM, 0);

        if (this->socketFd < 0) {
            this->socketRelease();
            LogFactory::get()->error(Utility::stringFormat("[Socket] socketFd initialization failed: [%d] %s", errno, strerror(errno)));
            return XCF_INVALID_RESULT;
        }

        // set socket options
        if (this->setNonBlock() < 0 || this->setKeepAlive() < 0 || this->setReuseAddr() < 0) {
            this->socketRelease();
            return XCF_INVALID_RESULT;
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
                LogFactory::get()->error(Utility::stringFormat("[Socket] socket binding failed: [%d] %s", errno, strerror(errno)));
                return XCF_INVALID_RESULT;
            }
            if (listen(this->socketFd, XCF_SOCK_LISTEN_BACKLOG) < 0) {
                this->socketRelease();
                LogFactory::get()->error(Utility::stringFormat("[Socket] socket listen failed: [%d] %s", errno, strerror(errno)));
                return XCF_INVALID_RESULT;
            }
        } else if (this->socketEndType == SocketEndType::CLIENT) {
            // client: connect
            if (connect(this->socketFd, (struct sockaddr *) &this->socketAddr, sizeof(this->socketAddr)) < 0) {
                if (errno != EINPROGRESS) { // non-blocking socket connect, EINPROGRESS would be returned
                    this->socketRelease();
                    LogFactory::get()->error(Utility::stringFormat("[Socket] socket connect failed: [%d] %s", errno, strerror(errno)));
                    return XCF_INVALID_RESULT;
                }
            }
        } else {
            LogFactory::get()->warn("[Socket] \"init\" called when socket already inited ...");
        }

        this->socketStatus = SocketStatus::CONNECTED;
    }

    return XCF_VALID_RESULT;
}

int32_t Socket::socketRelease() {
    if (this->socketFd > 0) {
        close(this->socketFd);
    }
    this->socketStatus = SocketStatus::CLOSED;
    this->socketFd = XCF_INVALID_SOCKET_FD;
    bzero(&this->socketAddr, sizeof(this->socketAddr));

    return XCF_VALID_RESULT;
}

int32_t Socket::read(char *buffer, int32_t length) {
    int32_t received = recv(this->socketFd, buffer, length, 0);
    if (received < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] error in recv: [%d] %s", errno, strerror(errno)));
    }
    return received;
}

int32_t Socket::read(SocketBuffer *buffer, int32_t length) {
    int32_t received = recv(this->socketFd, buffer->getBuffer(), length, 0);
    if (received < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] error in recv: [%d] %s", errno, strerror(errno)));
    }
    return received;
}

int32_t Socket::write(char *buffer) {
    int32_t transmitted = send(this->socketFd, buffer, strlen(buffer), 0);
    if (transmitted < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] error in send: [%d] %s", errno, strerror(errno)));
    }
    return transmitted;
}

int32_t Socket::write(SocketBuffer *buffer) {
    int32_t transmitted = send(this->socketFd, buffer->getBuffer(), strlen(buffer->getBuffer()), 0);
    if (transmitted < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] error in send: [%d] %s", errno, strerror(errno)));
    }
    return transmitted;
}

int32_t Socket::setWriteBuffSize(int32_t bufferSize) {
    int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_SNDBUF, (const void *) &bufferSize, sizeof(socklen_t));
    if (setResult < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] setWriteBuffSize failed: [%d] %s", errno, strerror(errno)));
    }
    return setResult;
}

int32_t Socket::setNonBlock() {
    int32_t flags = fcntl(this->socketFd, F_GETFL);
    int32_t setResult = fcntl(this->socketFd, F_SETFL, flags | O_NONBLOCK);
    if (setResult < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] setNonBlock failed: [%d] %s", errno, strerror(errno)));
    }
    return setResult;
}

int32_t Socket::setKeepAlive() {
    int32_t flags = 1;
    int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(int32_t));
    if (setResult < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] setKeepAlive failed: [%d] %s", errno, strerror(errno)));
    }
    return setResult;
}

int32_t Socket::setReuseAddr() {
    int32_t flags = 1;
    int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int32_t));
    if (setResult < 0) {
        LogFactory::get()->error(Utility::stringFormat("[Socket] setReuseAddr failed: [%d] %s", errno, strerror(errno)));
    }
    return setResult;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* SocketPool
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
SocketPool::SocketPool(): socketPool(new SocketPoolMap()) {}

SocketPool::~SocketPool() {
    this->clearSockets();
    delete this->socketPool;
}

DEF_NS_XCF_END
