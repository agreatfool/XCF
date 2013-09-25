#ifndef XCF_SERVERMAINTHREAD_H_
#define XCF_SERVERMAINTHREAD_H_

#include "../../../../Common.h"
#include "../../../thread/Thread.h"
#include "../../../event/ev/EventIo.h"

DEF_NS_XCF_BEGIN

class Socket;

class ServerMainThread: public Thread, public EventIo {
    public:
        ServerMainThread();
        virtual ~ServerMainThread();
        virtual int32_t prepareToRun();
        virtual void run();
        virtual std::string getThreadName();
        virtual std::string getEventName();
        static void acceptCallback(EventLoop *acceptLoop, EventIoWatcher *acceptWatcher, int revents);
    protected:
        virtual bool canBeBlocked();
        virtual void process();
        Socket *serverSocket;
};

DEF_NS_XCF_END

#endif /* XCF_SERVERMAINTHREAD_H_ */
