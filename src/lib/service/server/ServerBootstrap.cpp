#include "ServerBootstrap.h"
#include "threads/ServerMainThread.h"

DEF_NS_XCF_BEGIN

ServerBootstrap *ServerBootstrap::instance = NULL;

ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
    Bootstrap(protocolType, host, port), mainThread(NULL) {}

ServerBootstrap::~ServerBootstrap() {
    this->stop();
}

ServerBootstrap *ServerBootstrap::init(uint16_t protocolType, std::string host, uint16_t port) {
    if (Utility::isNullPtr(ServerBootstrap::instance)) {
        ServerBootstrap::instance = new ServerBootstrap(protocolType, host, port);
    }
    return ServerBootstrap::instance;
}

ServerBootstrap *ServerBootstrap::get() {
    return ServerBootstrap::instance;
}

int32_t ServerBootstrap::start() {
    this->eventIo->addWatcher(this->serverSocket->getSocketFd(), ServerBootstrap::acceptCallback);
    this->eventIo->startLoop();
    return XCF_VALID_RESULT;
}

int32_t ServerBootstrap::stop() {
    this->eventIo->stopLoop();
    this->socketPool->clearSockets();
    return XCF_VALID_RESULT;
}

ServerMainThread *ServerBootstrap::getMainThread() {
    return this->mainThread;
}

void ServerBootstrap::readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents) {
    if (EV_ERROR & revents) {
        LogFactory::get()->error("[ServerBootstrap] readCallback: got invalid event!");
        return;
    }

    int32_t socketFd = readWatcher->fd;
    Socket *socket = ServerBootstrap::get()->getSocketPool()->getSocket(socketFd);
    if (Utility::isNullPtr(socket)) {
        LogFactory::get()->error("[ServerBootstrap] readCallback: socket not found!");
        return;
    }

    SocketBuffer *buffer = new SocketBuffer();
    int32_t received = socket->read(buffer, XCF_SOCK_BUFFER_LENGTH);

    if (received < 0) {
        // error
        return;
    }

    if (received == 0) {
        // stop and free socket | watcher if client socket closed
        ServerBootstrap::get()->getEventIo()->removeWatcher(socketFd);
        ServerBootstrap::get()->getSocketPool()->removeSocket(socketFd);
        LogFactory::get()->info("[ServerBootstrap] readCallback: client closed!");
        return;
    } else {
        LogFactory::get()->info(
            Utility::stringFormat("[ServerBootstrap] readCallback: message: %s", buffer->getBuffer())
        );
    }

    // send message back to the client
    socket->write(buffer);
    delete buffer;
}

DEF_NS_XCF_END
