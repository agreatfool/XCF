#include "ServerMainThread.h"
#include "../../../socket/Socket.h"
#include "../../../log/Log.h"
#include "../../../utility/Utility.h"

DEF_NS_XCF_BEGIN

ServerMainThread::ServerMainThread():
    Thread(), EventIo(), serverSocket(NULL) {}

ServerMainThread::~ServerMainThread() {
    delete serverSocket;
}

bool ServerMainThread::canBeBlocked() { return false; }

void ServerMainThread::process() { /* no logic to process here */ }

void ServerMainThread::run() {
    LogFactory::get()->info(Utility::stringFormat("[Thread] Thread %d start to run ...", this->getNumericThreadId()));
    this->startLoop();
}

int32_t ServerMainThread::prepareToRun() {
    this->serverSocket = new Socket(
        ServerBootstrap::get()->getHost(),
        ServerBootstrap::get()->getPort(),
        ServerBootstrap::get()->getSocketProtocolType(),
        SocketEndType::SERVER
    );
    if (this->serverSocket->getSocketStatus() < SocketStatus::CONNECTED) {
        LogFactory::get()->error("[ServerBootstrap] server socket init failed!");
        return XCF_INVALID_RESULT;
    } else {
        this->addWatcher(this->serverSocket->getSocketFd(), ServerMainThread::acceptCallback);
        return XCF_VALID_RESULT;
    }
}

void ServerMainThread::acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents) {
    if (EV_ERROR & revents) {
        LogFactory::get()->error("[ServerBootstrap] acceptCallback: got invalid event!");
        return;
    }

    Socket *client = Socket::socketAccept(acceptWatcher->fd, SocketProtocol::TCP);
    if (Utility::isNullPtr(client)) {
        LogFactory::get()->error("[ServerBootstrap] acceptCallback: socket cannot be accepted!");
        return;
    }

    ServerBootstrap *server = ServerBootstrap::get();

    server->getSocketPool()->addSocket(client);
    ServerReaderThread *thread = (ServerReaderThread *) server->getReaderThreadPool()->next();
    thread->addWatcher(client->getSocketFd(), ServerReaderThread::readCallback);

    LogFactory::get()->info(
        Utility::stringFormat(
            "[ServerBootstrap] Successfully connected with client. %d connections established!",
            server->getSocketPool()->getPoolSize()
        )
    );
}

DEF_NS_XCF_END
