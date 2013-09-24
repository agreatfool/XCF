#include "ServerWorkerThread.h"

#include "../../../socket/Socket.h"

DEF_NS_XCF_BEGIN

ServerWorkerThread::ServerWorkerThread(): Thread(), works(new Queue<Socket>()) {}

ServerWorkerThread::~ServerWorkerThread() {}

int32_t ServerWorkerThread::prepareToRun() { return XCF_VALID_RESULT; }

bool ServerWorkerThread::canBeBlocked() {
    if (this->works->empty()) {
        return true;
    } else {
        return false;
    }
}

void ServerWorkerThread::process() {
    // TODO
    // 因为ServerBootstrap在启动的时候初始化的工作线程是：ServerWorkerThread
    // 那么在这个线程类中，如何填入客制化的业务逻辑？
}

DEF_NS_XCF_END
