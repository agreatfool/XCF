#include "ServerBootstrap.h"
#include "threads/ServerMainThread.h"
#include "threads/ServerReaderThread.h"
#include "threads/ServerWorkerThread.h"
#include "../../thread/ThreadPool.h"
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
        workerThread->run();
        workerThreadPool->addThread(workerThread);
    }

    // start to run main thread
    this->mainThread->init();
    this->mainThread->run();

    return XCF_VALID_RESULT;
}

int32_t ServerBootstrap::stop() {

    // FIXME
    // stop main thread
    // stop workers & readers
    // clear thread pool * 2
    // clear socket pool
    // etc...

//    this->socketPool->clearSockets();

    return XCF_VALID_RESULT;
}

void ServerBootstrap::readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents) {
    if (EV_ERROR & revents) {
        LogFactory::get()->error("[ServerBootstrap] readCallback: got invalid event!");
        return;
    }

    int32_t socketFd = readWatcher->fd;
    Socket *socket = ServerBootstrap::get()->getSocketPool()->getSocket(socketFd);
    if (Utility::isNullPtr(socket)) {
        LogFactory::get()->error("[ServerBootstrap] readCallback: socket not found!");
        return;
    }

    SocketBuffer *buffer = new SocketBuffer();
    int32_t received = socket->read(buffer, XCF_SOCK_BUFFER_LENGTH);

    if (received < 0) {
        // error
        return;
    }

    if (received == 0) {
        // stop and free socket | watcher if client socket closed
        ServerBootstrap::get()->getEventIo()->removeWatcher(socketFd);
        ServerBootstrap::get()->getSocketPool()->removeSocket(socketFd);
        LogFactory::get()->info("[ServerBootstrap] readCallback: client closed!");
        return;
    } else {
        LogFactory::get()->info(
            Utility::stringFormat("[ServerBootstrap] readCallback: message: %s", buffer->getBuffer())
        );
    }

    // send message back to the client
    socket->write(buffer);
    delete buffer;
}

DEF_NS_XCF_END
