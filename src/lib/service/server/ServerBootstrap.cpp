#include "ServerBootstrap.h"
#include "threads/ServerMainThread.h"
#include "threads/ServerReaderThread.h"
#include "threads/ServerWorkerThread.h"
#include "../../utility/Utility.h"

DEF_NS_XCF_BEGIN

ServerBootstrap *ServerBootstrap::instance = NULL;

ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
    Bootstrap(protocolType, host, port),
    mainThread(NULL), readerThreadPool(NULL), workerThreadPool(NULL) {}

ServerBootstrap::~ServerBootstrap() {
    this->stop();
}

ServerBootstrap *ServerBootstrap::init(uint16_t protocolType, std::string host, uint16_t port) {
    if (Utility::isNullPtr(ServerBootstrap::instance)) {
        ServerBootstrap::instance = new ServerBootstrap(protocolType, host, port);
    }
    return ServerBootstrap::instance;
}

ServerBootstrap *ServerBootstrap::get() {
    return ServerBootstrap::instance;
}

int32_t ServerBootstrap::start() {
    uint16_t cpuNum = Utility::getCpuNum();

    // new main thread
    this->mainThread = new ServerMainThread();

    // new reader threads
    uint16_t readerThreadPoolSize = cpuNum * XCF_READER_THREAD_FACTOR;
    this->readerThreadPool = new FixedThreadPool<ServerReaderThread>(readerThreadPoolSize);

    // new worker threads
    this->workerThreadPool = new IdleThreadPool();
    for (int i = 0; i < cpuNum * XCF_WORKER_THREAD_FACTOR; ++i) {
        Thread *workerThread = new ServerWorkerThread();
        workerThread->init();
        workerThreadPool->addThread(workerThread);
    }

    // start to run main thread
    this->mainThread->init();

    // wait main thread processing here
    ThreadUtil::joinThread(this->mainThread->getThreadId());

    return XCF_OK;
}

int32_t ServerBootstrap::stop() {

    // FIXME
    // stop main thread
    // stop workers & readers
    // clear thread pool * 2
    // clear socket pool
    // etc...

//    this->socketPool->clearSockets();

    return XCF_OK;
}

DEF_NS_XCF_END
