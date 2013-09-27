#include "ServerMainThread.h"
#include "../ServerBootstrap.h"
#include "ServerReaderThread.h"
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

std::string ServerMainThread::getThreadName() { return "ServerMainThread"; }

std::string ServerMainThread::getEventName() { return "ServerMainThreadEvent"; }

void ServerMainThread::run() {
    LogFactory::get()->info(Utility::stringFormat("[ServerMainThread] Thread %d start to run ...", this->getNumericThreadId()));
    this->startLoop();
}

int32_t ServerMainThread::prepareToRun() {
    ServerBootstrap *bootstrap = ServerBootstrap::get();
    this->serverSocket = new Socket(
        bootstrap->getHost(),
        bootstrap->getPort(),
        bootstrap->getSocketProtocolType(),
        SocketEndType::SERVER
    );
    if (this->serverSocket->getSocketStatus() < SocketStatus::CONNECTED) {
        LogFactory::get()->error("[ServerMainThread] server socket init failed!");
        return XCF_ERR;
    } else {
        this->addWatcher(this->serverSocket->getSocketFd(), ServerMainThread::acceptCallback);
        return XCF_OK;
    }
}

void ServerMainThread::acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents) {
    if (EV_ERROR & revents) {
        LogFactory::get()->error("[ServerMainThread] acceptCallback: got invalid event!");
        return;
    }

    Socket *client = Socket::socketAccept(acceptWatcher->fd, SocketProtocol::TCP);
    if (Utility::isNullPtr(client)) {
        LogFactory::get()->error("[ServerMainThread] acceptCallback: socket cannot be accepted!");
        return;
    }

    ServerBootstrap *server = ServerBootstrap::get();

    server->getSocketPool()->addSocket(client);
//    ServerReaderThread *thread = (ServerReaderThread *) server->getReaderThreadPool()->getThreadViaShard(client->getSocketFd());
//    thread->addWatcher(client->getSocketFd(), ServerReaderThread::readCallback);
    server->getReaderThread()->addWatcher(client->getSocketFd(), ServerReaderThread::readCallback);
    server->getReaderThread()->startLoop(); // FIXME is there any way to start the loop? in ServerReaderThread::run? in Event::addWatcher?

    LogFactory::get()->info(
        Utility::stringFormat(
            "[ServerMainThread] Successfully connected with client. %d connections established!",
            server->getSocketPool()->getPoolSize()
        )
    );
}

DEF_NS_XCF_END
