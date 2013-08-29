#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log *Bootstrap::logger = LogFactory::get();

    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType) {}

    Bootstrap::~Bootstrap() {}

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    EventIo *ServerBootstrap::eventIo = new EventIo();
    SocketPool *ServerBootstrap::socketPool = new SocketPool();

    ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
        Bootstrap(protocolType), serverSocket(NULL)
    {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        if (this->serverSocket->getSocketStatus() < SocketStatus::CONNECTED) {
            ServerBootstrap::logger->error("[ServerBootstrap] server socket init failed!");
            exit(1);
        }
        // TODO design the pipline mode like the netty!
    }

    ServerBootstrap::~ServerBootstrap() {
        ServerBootstrap::eventIo->stopLoop();
        ServerBootstrap::socketPool->clearSockets();
        delete serverSocket;
    }

    int32_t ServerBootstrap::start() {
        ServerBootstrap::eventIo->addWatcher(this->serverSocket->getSocketFd(), ServerBootstrap::acceptCallback);
        ServerBootstrap::eventIo->startLoop();
        return VALID_RESULT;
    }

    void ServerBootstrap::acceptCallback(EventLoop *acceptLoop, EventWatcher *acceptWatcher, int revents) {
        if (EV_ERROR & revents) {
            ServerBootstrap::logger->error("[ServerBootstrap] acceptCallback: got invalid event!");
            return;
        }

        Socket *client = Socket::socketAccept(acceptWatcher->fd, SocketProtocol::TCP);
        if (Utility::isNullPtr(client)) {
            ServerBootstrap::logger->error("[ServerBootstrap] acceptCallback: socket cannot be accepted!");
            return;
        }

        ServerBootstrap::socketPool->addSocket(client);
        ServerBootstrap::eventIo->addWatcher(client->getSocketFd(), ServerBootstrap::readCallback);

        ServerBootstrap::logger->info(
            Utility::stringFormat(
                "[ServerBootstrap] Successfully connected with client. %d connections established!",
                ServerBootstrap::socketPool->getPoolSize()
            )
        );
    }

    void ServerBootstrap::readCallback(EventLoop *readLoop, EventWatcher *readWatcher, int revents) {
        if (EV_ERROR & revents) {
            ServerBootstrap::logger->error("[ServerBootstrap] readCallback: got invalid event!");
            return;
        }

        int32_t socketFd = readWatcher->fd;
        Socket *socket = ServerBootstrap::socketPool->getSocket(socketFd);
        if (Utility::isNullPtr(socket)) {
            ServerBootstrap::logger->error("[ServerBootstrap] readCallback: socket not found!");
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
            ServerBootstrap::eventIo->removeWatcher(socketFd);
            ServerBootstrap::socketPool->removeSocket(socketFd);
            ServerBootstrap::logger->info("[ServerBootstrap] readCallback: client closed!");
            return;
        } else {
            ServerBootstrap::logger->info(
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
    EventIo *ClientBootstrap::eventIo = new EventIo();
    SocketPool *ClientBootstrap::socketPool = new SocketPool();

    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {}

    ClientBootstrap::~ClientBootstrap() {
        ClientBootstrap::eventIo->stopLoop();
        ClientBootstrap::socketPool->clearSockets();
    }

    int32_t ClientBootstrap::start() {
        ServerBootstrap::eventIo->startLoop();
        return VALID_RESULT;
    }

} /* namespace XCF */
