#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType), logger(LogFactory::get()) {}

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
        Bootstrap(protocolType), serverSocket(NULL)
    {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        if (this->serverSocket->listen() < 0) {
            // FIXME error message not done, logger not initialized, should we delete the logger pointer in the ~ function? test it!
            this->logger->error("[ServerBootstrap] ");
            exit(1);
        }
        // TODO design the pipline mode like the netty!
    }

    ServerBootstrap::~ServerBootstrap() {
        delete serverSocket;
    }

    int32_t ServerBootstrap::start() {
        return 1;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ClientBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {};

    int32_t ClientBootstrap::start() {
        return 1;
    }

} /* namespace XCF */
