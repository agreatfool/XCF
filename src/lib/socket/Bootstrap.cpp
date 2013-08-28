#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType), logger(LogFactory::get()) {}

    Bootstrap::~Bootstrap() {
        delete this->logger;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
        Bootstrap(protocolType),
        clientCount(0), serverSocket(NULL), acceptLoop(ev_default_loop(0))
    {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        if (this->serverSocket->socketInit() < 0) {
            this->logger->error("[ServerBootstrap] server socket init failed!");
            exit(1);
        }
        // TODO design the pipline mode like the netty!
    }

    ServerBootstrap::~ServerBootstrap() {
        delete serverSocket;
    }

    int32_t ServerBootstrap::start() {
        ev_io_init(&this->acceptWatcher, ServerBootstrap::acceptCallback, this->serverSocket->getSocketFd(), EV_READ);
        ev_io_start(this->acceptLoop, &this->acceptWatcher);
        ev_run(this->acceptLoop, 0);

        return VALID_RESULT;
    }

    void ServerBootstrap::acceptCallback(struct ev_loop *acceptLoop, struct ev_io *acceptWatcher, int revents) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int32_t clientFd;
        struct ev_io *clientWatcher = (struct ev_io *) malloc (sizeof(struct ev_io));

        if (EV_ERROR & revents) {
            std::cout << "[ServerBootstrap] acceptCallback: got invalid event!" << std::endl;
            return;
        }

        // accept client request
        clientFd = accept(acceptWatcher->fd, (struct sockaddr *) &clientAddr, &clientLen);

        if (clientFd < 0) {
            std::cout << "[ServerBootstrap] acceptCallback: accept error!" << std::endl;
            return;
        }

//        this->clientCount++; // Increment total clients count
//        this->logger->info("[ServerBootstrap] Successfully connected with client.");
//        this->logger->info("[ServerBootstrap] " + this->clientCount + " client(s) connected.");
        std::cout << "[ServerBootstrap] Successfully connected with client." << std::endl;

        // Initialize and start watcher to read client requests
        ev_io_init(clientWatcher, ServerBootstrap::readCallback, clientFd, EV_READ);
        ev_io_start(acceptLoop, clientWatcher);
    }

    void ServerBootstrap::readCallback(struct ev_loop *readLoop, struct ev_io *readWatcher, int revents) {
        char buffer[SOCK_BUFFER_LENGTH];
        ssize_t read;

        if (EV_ERROR & revents) {
            std::cout << "[ServerBootstrap] readCallback: got invalid event!" << std::endl;
            return;
        }

        // receive message from client socket
        read = recv(readWatcher->fd, buffer, SOCK_BUFFER_LENGTH, 0);

        if (read < 0) {
            std::cout << "[ServerBootstrap] readCallback: read error!" << std::endl;
            return;
        }

        if (read == 0) {
            // stop and free watchet if client socket is closing
            ev_io_stop(readLoop, readWatcher);
            delete readWatcher;
//            this->logger->info("[ServerBootstrap] readCallback: peer might closing!");
//            this->clientCount--; // decrement total_clients count
//            this->logger->info(this->clientCount + " client(s) connected.");
            std::cout << "[ServerBootstrap] readCallback: peer might closing!" << std::endl;
            return;
        } else {
            std::cout << "[ServerBootstrap] readCallback: message: " << buffer << std::endl;
        }

        // send message bach to the client
        send(readWatcher->fd, buffer, read, 0);
        bzero(buffer, read);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ClientBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {}

    ClientBootstrap::~ClientBootstrap() {}

    int32_t ClientBootstrap::start() {
        return 1;
    }

} /* namespace XCF */
