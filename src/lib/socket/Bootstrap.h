#ifndef XCF_BOOTSTRAP_H_
#define XCF_BOOTSTRAP_H_

#include "../../Declare.h"
#include "Socket.h"
#include "../log/Log.h"
#include "../event/Event.h"

namespace XCF {

    class Bootstrap {
        public:
            Bootstrap(uint16_t protocolType);
            virtual ~Bootstrap();
            virtual int32_t start() = 0;
        protected:
            uint16_t socketProtocolType;
            Log*     logger;
    };

    class ServerBootstrap: public Bootstrap {
        public:
            ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port);
            virtual ~ServerBootstrap();
            static EventIo *eventIo;
            int32_t start();
            static void acceptCallback(struct ev_loop *acceptLoop, struct ev_io *acceptWatcher, int revents);
            static void readCallback(struct ev_loop *readLoop, struct ev_io *readWatcher, int revents);
        protected:
            Socket         *serverSocket;
    };

    class ClientBootstrap: public Bootstrap {
        public:
            ClientBootstrap(uint16_t protocolType);
            virtual ~ClientBootstrap();
            int32_t start();
        protected:
    };

} /* namespace XCF */

#endif /* XCF_BOOTSTRAP_H_ */
