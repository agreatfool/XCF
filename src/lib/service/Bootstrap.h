#ifndef XCF_BOOTSTRAP_H_
#define XCF_BOOTSTRAP_H_

#include "../../Common.h"

DEF_NS_XCF_BEGIN

class SocketPool;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* Bootstrap
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class Bootstrap {
    public:
        Bootstrap(uint16_t protocolType, std::string host, uint16_t port);
        virtual ~Bootstrap();
        virtual int32_t start() = 0;
        virtual int32_t stop() = 0;
        uint16_t getSocketProtocolType();
        std::string getHost();
        uint16_t getPort();
        SocketPool *getSocketPool();
    protected:
        uint16_t    socketProtocolType;
        std::string host;
        uint16_t    port;
        SocketPool  *socketPool;
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* inline Implementation
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline uint16_t Bootstrap::getSocketProtocolType() {
    return this->socketProtocolType;
}
inline std::string Bootstrap::getHost() {
    return this->host;
}
inline uint16_t Bootstrap::getPort() {
    return this->port;
}
inline SocketPool *Bootstrap::getSocketPool() {
    return this->socketPool;
}

DEF_NS_XCF_END

#endif /* XCF_BOOTSTRAP_H_ */
