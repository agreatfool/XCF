#include "ServerReaderThread.h"
#include "../../../socket/Socket.h"
#include "../../../log/Log.h"
#include "../../../utility/Utility.h"

DEF_NS_XCF_BEGIN

ServerReaderThread::ServerReaderThread():
    Thread(), EventIo() {}

ServerReaderThread::~ServerReaderThread() {}

bool ServerReaderThread::canBeBlocked() { return false; }

void ServerReaderThread::process() { /* no logic to process here */ }

void ServerReaderThread::run() {
    LogFactory::get()->info(Utility::stringFormat("[Thread] Thread %d start to run ...", this->getNumericThreadId()));
    this->startLoop();
}

int32_t ServerReaderThread::prepareToRun() { return XCF_VALID_RESULT; }

void ServerReaderThread::readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents) {
    if (EV_ERROR & revents) {
        LogFactory::get()->error("[ServerReaderThread] readCallback: got invalid event!");
        return;
    }

    int32_t socketFd = readWatcher->fd;
    Socket *socket = ServerBootstrap::get()->getSocketPool()->getSocket(socketFd);
    if (Utility::isNullPtr(socket)) {
        LogFactory::get()->error("[ServerReaderThread] readCallback: socket not found!");
        return;
    }

    // FIXME read detail? pipline?
    // FIXME 修正socket，将SocketBuffer内嵌于Socket，并提供clear buffer这样的工具函数
    SocketBuffer *buffer = new SocketBuffer();
    int32_t received = socket->read(buffer, XCF_SOCK_BUFFER_LENGTH);

    if (received < 0) {
        // error
        return;
    }

    if (received == 0) {
        // FIXME 因为需要在根据socketFd编号从ThreadPool内获得到Thread，所以ThreadPool的尺寸必须是固定的
        // 根据这个尺寸进行shard，获得对应的Thread，感觉ThreadPool应该更名为FixedThreadPool
        // stop and free socket | watcher if client socket closed
        ServerBootstrap::get()->getEventIo()->removeWatcher(socketFd);
        ServerBootstrap::get()->getSocketPool()->removeSocket(socketFd);
        LogFactory::get()->info("[ServerReaderThread] readCallback: client closed!");
        return;
    } else {
        LogFactory::get()->info(
            Utility::stringFormat("[ServerReaderThread] readCallback: message: %s", buffer->getBuffer())
        );
        // send message back to the client
        socket->write(buffer);
        // FIXME logics other than "echo"
    }

    delete buffer;
}

DEF_NS_XCF_END
