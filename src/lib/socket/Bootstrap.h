#ifndef XCF_BOOTSTRAP_H_
#define XCF_BOOTSTRAP_H_

#include "Socket.h"

namespace XCF {

    class Bootstrap {
        public:
            Bootstrap();
            Bootstrap(uint16_t protocolType);
            virtual ~Bootstrap();
            virtual int32_t start();
        protected:
            uint16_t socketProtocolType;
    };

    class ServerBootstrap: public Bootstrap {
        public:
            ServerBootstrap();
            ServerBootstrap(uint16_t protocolType);
            virtual ~ServerBootstrap();
            int32_t bind(std::string host, uint16_t port);
        protected:
            Socket* serverSocket;
    };

    class ClientBootstrap: public Bootstrap {
        public:
            ClientBootstrap();
            ClientBootstrap(uint16_t protocolType);
            virtual ~ClientBootstrap();
        protected:
    };

} /* namespace XCF */

#endif /* XCF_BOOTSTRAP_H_ */
