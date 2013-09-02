#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo *Bootstrap::eventIo = new EventIo();
    SocketPool *Bootstrap::socketPool = new SocketPool();
    Log *Bootstrap::logger = LogFactory::get();

    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType) {}

    Bootstrap::~Bootstrap() {}

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
        Bootstrap(protocolType), serverSocket(NULL)
    {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        if (this->serverSocket->getSocketStatus() < SocketStatus::CONNECTED) {
            ServerBootstrap::getLogger()->error("[ServerBootstrap] server socket init failed!");
            exit(1);
        }
        // TODO design the pipline mode like the netty!
    }

    ServerBootstrap::~ServerBootstrap() {
        this->stop();
        delete serverSocket;
    }

    int32_t ServerBootstrap::start() {
        ServerBootstrap::getEventIo()->addWatcher(this->serverSocket->getSocketFd(), ServerBootstrap::acceptCallback);
        ServerBootstrap::getEventIo()->startLoop();
        return VALID_RESULT;
    }

    int32_t ServerBootstrap::stop() {
        ServerBootstrap::getEventIo()->stopLoop();
        ServerBootstrap::getSocketPool()->clearSockets();
        return VALID_RESULT;
    }

    void ServerBootstrap::acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents) {
        if (EV_ERROR & revents) {
            ServerBootstrap::getLogger()->error("[ServerBootstrap] acceptCallback: got invalid event!");
            return;
        }

        Socket *client = Socket::socketAccept(acceptWatcher->fd, SocketProtocol::TCP);
        if (Utility::isNullPtr(client)) {
            ServerBootstrap::getLogger()->error("[ServerBootstrap] acceptCallback: socket cannot be accepted!");
            return;
        }

        ServerBootstrap::getSocketPool()->addSocket(client);
        ServerBootstrap::getEventIo()->addWatcher(client->getSocketFd(), ServerBootstrap::readCallback);

        ServerBootstrap::getLogger()->info(
            Utility::stringFormat(
                "[ServerBootstrap] Successfully connected with client. %d connections established!",
                ServerBootstrap::getSocketPool()->getPoolSize()
            )
        );
    }

    void ServerBootstrap::readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents) {
        if (EV_ERROR & revents) {
            ServerBootstrap::getLogger()->error("[ServerBootstrap] readCallback: got invalid event!");
            return;
        }

        int32_t socketFd = readWatcher->fd;
        Socket *socket = ServerBootstrap::getSocketPool()->getSocket(socketFd);
        if (Utility::isNullPtr(socket)) {
            ServerBootstrap::getLogger()->error("[ServerBootstrap] readCallback: socket not found!");
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
            ServerBootstrap::getEventIo()->removeWatcher(socketFd);
            ServerBootstrap::getSocketPool()->removeSocket(socketFd);
            ServerBootstrap::getLogger()->info("[ServerBootstrap] readCallback: client closed!");
            return;
        } else {
            ServerBootstrap::getLogger()->info(
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
    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {}

    ClientBootstrap::~ClientBootstrap() {
        this->stop();
    }

    int32_t ClientBootstrap::start() {
        ServerBootstrap::getEventIo()->startLoop();
        return VALID_RESULT;
    }

    int32_t ClientBootstrap::stop() {
        ClientBootstrap::getEventIo()->stopLoop();
        ClientBootstrap::getSocketPool()->clearSockets();
        return VALID_RESULT;
    }

} /* namespace XCF */
