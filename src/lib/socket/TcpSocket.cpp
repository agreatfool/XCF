#include "TcpSocket.h"

namespace XCF {

    TcpSocket::TcpSocket(std::string host, uint16_t port):
        Socket(host, port, SocketProtocol::TCP) {}

    TcpSocket::~TcpSocket() {}

    int32_t TcpSocket::createServer() {

    }

    int32_t TcpSocket::createClient() {

    }

} /* namespace XCF */
