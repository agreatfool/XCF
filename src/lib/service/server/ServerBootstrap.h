#ifndef XCF_SERVERBOOTSTRAP_H_
#define XCF_SERVERBOOTSTRAP_H_

#include "../../../Common.h"
#include "../Bootstrap.h"

DEF_NS_XCF_BEGIN

class ServerMainThread;
class ThreadPool;

class ServerBootstrap: public Bootstrap {
    public:
        virtual ~ServerBootstrap();
        static ServerBootstrap *init(uint16_t protocolType, std::string host, uint16_t port);
        static ServerBootstrap *get();
        int32_t start();
        int32_t stop();
        ServerMainThread *getMainThread();
        ThreadPool *getReaderThreadPool();
        ThreadPool *getWorkerThreadPool();
        static void readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents);
    protected:
        ServerMainThread *mainThread;
        ThreadPool *readerThreadPool;
        ThreadPool *workerThreadPool;
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
inline ThreadPool *ServerBootstrap::getReaderThreadPool() {
    return this->readerThreadPool;
}
inline ThreadPool *ServerBootstrap::getWorkerThreadPool() {
    return this->workerThreadPool;
}

DEF_NS_XCF_END

#endif /* XCF_SERVERBOOTSTRAP_H_ */
