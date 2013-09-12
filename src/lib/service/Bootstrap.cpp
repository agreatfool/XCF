#include "Bootstrap.h"
#include "../socket/Socket.h"
#include "../event/Event.h"

DEF_NS_XCF_BEGIN

Bootstrap::Bootstrap(uint16_t protocolType, std::string host, uint16_t port):
    socketProtocolType(protocolType),
    host(host),
    port(port),
    socketPool(new SocketPool()) {}

Bootstrap::~Bootstrap() {}


DEF_NS_XCF_END
