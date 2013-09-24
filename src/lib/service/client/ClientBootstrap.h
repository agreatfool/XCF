#ifndef XCF_CLIENTBOOTSTRAP_H_
#define XCF_CLIENTBOOTSTRAP_H_

#include "../../../Common.h"
#include "../Bootstrap.h"

DEF_NS_XCF_BEGIN

class EventIo;

class ClientBootstrap: public Bootstrap {
    public:
        virtual ~ClientBootstrap();
        static ClientBootstrap *init(uint16_t protocolType);
        static ClientBootstrap *get();
        int32_t start();
        int32_t stop();
        EventIo *getEventIo();
    protected:
        static ClientBootstrap *instance;
        EventIo *eventIo;
        ClientBootstrap(uint16_t protocolType);
        // Stop the compiler generating methods of copy the object
        ClientBootstrap(ClientBootstrap const& copy);            // Not Implemented
        ClientBootstrap& operator=(ClientBootstrap const& copy); // Not Implemented
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* inline implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline EventIo *ClientBootstrap::getEventIo() {
    return this->eventIo;
};

DEF_NS_XCF_END

#endif /* XCF_CLIENTBOOTSTRAP_H_ */
