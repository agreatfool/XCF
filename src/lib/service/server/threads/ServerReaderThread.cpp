#include "ServerReaderThread.h"
#include "../ServerBootstrap.h"
#include "../../../socket/Socket.h"
#include "../../../log/Log.h"
#include "../../../utility/Utility.h"
#include "../../../thread/ThreadUtil.h"

DEF_NS_XCF_BEGIN

ServerReaderThread::ServerReaderThread():
    Thread(), EventIo() {}

ServerReaderThread::~ServerReaderThread() {}

bool ServerReaderThread::canBeBlocked() { return false; }

void ServerReaderThread::process() { /* no logic to process here */ }

std::string ServerReaderThread::getThreadName() { return "ServerReaderThread"; }

std::string ServerReaderThread::getEventName() { return "ServerReaderThreadEvent"; }

//void ServerReaderThread::run() {
//    LogFactory::get()->info(Utility::stringFormat("[ServerReaderThread] Thread %d start to run ...", this->getNumericThreadId()));
//    this->addWatcher(XCF_STDIN_FD, EventIo::dummyCallback);
//    this->startLoop();
//}
void ServerReaderThread::run() {
    LogFactory::get()->info(Utility::stringFormat("[ServerReaderThread] Thread %d start to run ...", this->getNumericThreadId()));
//    this->startLoop();
//    ThreadUtil::joinThread(ServerBootstrap::get()->getMainThread()->getThreadId()); // wait, and do not exit the thread
    ThreadId bootstrapId = ServerBootstrap::get()->getThreadId();
    ThreadUtil::joinThread(&bootstrapId);
}

int32_t ServerReaderThread::prepareToRun() { return XCF_OK; }

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

    ServerBootstrap *server = ServerBootstrap::get();

    if (received == 0) {
        // stop and free socket & watcher if client socket closed
        ServerReaderThread *thread = (ServerReaderThread *) server->getReaderThreadPool()->getThreadViaShard(socketFd);
        thread->removeWatcher(socketFd);
        server->getSocketPool()->removeSocket(socketFd);
        LogFactory::get()->info("[ServerReaderThread] readCallback: client closed!");
        return;
    } else {
        // TODO
        // 1. 需要添加一个socket状态，假如socket在前一个worker线程中处理，并暂时未处理完成的话，这里读取就应该直接返回错误信息，表示前一个请求未完成
        // 2. socket要有手段确定已经把消息读完了
        // 3. 读完的消息将会被设成“处理中”的状态，并塞给worker线程
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
