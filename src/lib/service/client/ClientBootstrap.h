#ifndef XCF_CLIENTBOOTSTRAP_H_
#define XCF_CLIENTBOOTSTRAP_H_

#include "../../../Common.h"
#include "../Bootstrap.h"

DEF_NS_XCF_BEGIN

class ClientBootstrap: public Bootstrap {
    public:
        virtual ~ClientBootstrap();
        static ClientBootstrap *init(uint16_t protocolType);
        static ClientBootstrap *get();
        int32_t start();
        int32_t stop();
    protected:
        static ClientBootstrap *instance;
        ClientBootstrap(uint16_t protocolType);
        // Stop the compiler generating methods of copy the object
        ClientBootstrap(ClientBootstrap const& copy);            // Not Implemented
        ClientBootstrap& operator=(ClientBootstrap const& copy); // Not Implemented
};

DEF_NS_XCF_END

#endif /* XCF_CLIENTBOOTSTRAP_H_ */
