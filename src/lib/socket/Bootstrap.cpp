#include "Bootstrap.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Bootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Bootstrap::Bootstrap():
        socketProtocolType(SocketProtocol::TCP) {}

    Bootstrap::Bootstrap(uint16_t protocolType):
        socketProtocolType(protocolType) {}

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ServerBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ServerBootstrap::ServerBootstrap():
        Bootstrap(), serverSocket(NULL) {}

    ServerBootstrap::ServerBootstrap(uint16_t protocolType):
        Bootstrap(protocolType), serverSocket(NULL) {}

    ServerBootstrap::~ServerBootstrap() {
        delete serverSocket;
    }

    int32_t ServerBootstrap::bind(std::string host, uint16_t port) {
        this->serverSocket = new Socket(host, port, this->socketProtocolType, SocketEndType::SERVER);
        return this->serverSocket->listen();
    }

    int32_t ServerBootstrap::start() {

    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* ClientBootstrap
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    ClientBootstrap::ClientBootstrap():
        Bootstrap() {};

    ClientBootstrap::ClientBootstrap(uint16_t protocolType):
        Bootstrap(protocolType) {};

    int32_t ClientBootstrap::start() {

    }

} /* namespace XCF */
