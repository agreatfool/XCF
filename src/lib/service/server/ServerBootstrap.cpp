#include "ServerBootstrap.h"
#include "threads/ServerMainThread.h"
#include "threads/ServerReaderThread.h"
#include "threads/ServerWorkerThread.h"
#include "../../thread/ThreadUtil.h"
#include "../../utility/Utility.h"

DEF_NS_XCF_BEGIN

ServerBootstrap *ServerBootstrap::instance = NULL;

ServerBootstrap::ServerBootstrap(uint16_t protocolType, std::string host, uint16_t port):
    Bootstrap(protocolType, host, port),
    mainThread(NULL), readerThreadPool(NULL), workerThreadPool(NULL), readerThread(NULL) {}

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
//    uint16_t cpuNum = Utility::getCpuNum();

    // init reader threads
//    uint16_t readerThreadPoolSize = 1; // cpuNum * XCF_READER_THREAD_FACTOR;
//    this->readerThreadPool = new FixedThreadPool<ServerReaderThread>(readerThreadPoolSize);

    this->readerThread = new ServerReaderThread();
    this->readerThread->init();

    // init worker threads
//    uint16_t workerThreadPoolSize = 1; // cpuNum * XCF_WORKER_THREAD_FACTOR;
//    this->workerThreadPool = new IdleThreadPool();
//    for (int i = 0; i < workerThreadPoolSize; ++i) {
//        Thread *workerThread = new ServerWorkerThread();
//        workerThread->init();
//        workerThreadPool->addThread(workerThread);
//    }

    // init main thread
    this->mainThread = new ServerMainThread();
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
