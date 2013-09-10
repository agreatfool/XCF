#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType),
        eventIo(new EventIo()), socketPool(new SocketPool()) {}

    Bootstrap::~Bootstrap() {}

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ServerBootstrap *ServerBootstrap::instance = NULL;

    ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
        Bootstrap(protocolType), serverSocket(NULL)
    {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        if (this->serverSocket->getSocketStatus() < SocketStatus::CONNECTED) {
            LogFactory::get()->error("[ServerBootstrap] server socket init failed!");
            exit(1);
        }
        // TODO design the pipline mode like the netty!
    }

    ServerBootstrap::~ServerBootstrap() {
        this->stop();
        delete serverSocket;
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
        return VALID_RESULT;
    }

    int32_t ServerBootstrap::stop() {
        this->eventIo->stopLoop();
        this->socketPool->clearSockets();
        return VALID_RESULT;
    }

    void ServerBootstrap::acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents) {
        if (EV_ERROR & revents) {
            LogFactory::get()->error("[ServerBootstrap] acceptCallback: got invalid event!");
            return;
        }

        Socket *client = Socket::socketAccept(acceptWatcher->fd, SocketProtocol::TCP);
        if (Utility::isNullPtr(client)) {
            LogFactory::get()->error("[ServerBootstrap] acceptCallback: socket cannot be accepted!");
            return;
        }

        ServerBootstrap::get()->getSocketPool()->addSocket(client);
        ServerBootstrap::get()->getEventIo()->addWatcher(client->getSocketFd(), ServerBootstrap::readCallback);

        LogFactory::get()->info(
            Utility::stringFormat(
                "[ServerBootstrap] Successfully connected with client. %d connections established!",
                ServerBootstrap::get()->getSocketPool()->getPoolSize()
            )
        );
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
        int32_t received = socket->read(buffer, SOCK_BUFFER_LENGTH);

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

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ClientBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ClientBootstrap *ClientBootstrap::instance = NULL;

    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {}

    ClientBootstrap::~ClientBootstrap() {
        this->stop();
    }

    ClientBootstrap *ClientBootstrap::init(uint16_t protocolType) {
        if (Utility::isNullPtr(ClientBootstrap::instance)) {
            ClientBootstrap::instance = new ClientBootstrap(protocolType);
        }
        return ClientBootstrap::instance;
    }

    ClientBootstrap *ClientBootstrap::get() {
        return ClientBootstrap::instance;
    }

    int32_t ClientBootstrap::start() {
        this->eventIo->startLoop();
        return VALID_RESULT;
    }

    int32_t ClientBootstrap::stop() {
        this->eventIo->stopLoop();
        this->socketPool->clearSockets();
        return VALID_RESULT;
    }

} /* namespace XCF */
