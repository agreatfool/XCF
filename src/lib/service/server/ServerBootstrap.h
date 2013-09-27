#ifndef XCF_SERVERBOOTSTRAP_H_
#define XCF_SERVERBOOTSTRAP_H_

#include "../../../Common.h"
#include "../Bootstrap.h"
#include "../../thread/ThreadPool.h"

DEF_NS_XCF_BEGIN

class ServerMainThread;
class ServerReaderThread;

class ServerBootstrap: public Bootstrap {
    public:
        virtual ~ServerBootstrap();
        static ServerBootstrap *init(uint16_t protocolType, std::string host, uint16_t port);
        static ServerBootstrap *get();
        int32_t start();
        int32_t stop();
        ServerMainThread *getMainThread();
        FixedThreadPool<ServerReaderThread> *getReaderThreadPool();
        IdleThreadPool *getWorkerThreadPool();
        ServerReaderThread *getReaderThread();
        ThreadId getThreadId();
    protected:
        ServerMainThread  *mainThread;
        FixedThreadPool<ServerReaderThread> *readerThreadPool;
        IdleThreadPool *workerThreadPool;
        ServerReaderThread *readerThread;
        static ServerBootstrap *instance;
        ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port);
        // Stop the compiler generating methods of copy the object
        ServerBootstrap(ServerBootstrap const& copy);            // Not Implemented
        ServerBootstrap& operator=(ServerBootstrap const& copy); // Not Implemented
};

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- inline Implementations
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
inline ServerMainThread *ServerBootstrap::getMainThread() {
    return this->mainThread;
}
inline FixedThreadPool<ServerReaderThread> *ServerBootstrap::getReaderThreadPool() {
    return this->readerThreadPool;
}
inline IdleThreadPool *ServerBootstrap::getWorkerThreadPool() {
    return this->workerThreadPool;
}
inline ServerReaderThread *ServerBootstrap::getReaderThread() {
    return this->readerThread;
}
inline ThreadId ServerBootstrap::getThreadId() {
    return pthread_self();
}

DEF_NS_XCF_END

#endif /* XCF_SERVERBOOTSTRAP_H_ */
