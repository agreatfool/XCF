#include "ClientBootstrap.h"
#include "../../event/ev/EventIo.h"
#include "../../socket/Socket.h"
#include "../../utility/Utility.h"

DEF_NS_XCF_BEGIN

ClientBootstrap *ClientBootstrap::instance = NULL;

ClientBootstrap::ClientBootstrap(uint16_t protocolType):
    Bootstrap(protocolType, "", 0), eventIo(new EventIo()) {}

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
    return XCF_VALID_RESULT;
}

int32_t ClientBootstrap::stop() {
    this->eventIo->stopLoop();
    this->socketPool->clearSockets();
    return XCF_VALID_RESULT;
}

DEF_NS_XCF_END
