#ifndef XCF_SERVERWORKERTHREAD_H_
#define XCF_SERVERWORKERTHREAD_H_

#include "../../../../Common.h"
#include "../../../thread/Thread.h"

DEF_NS_XCF_BEGIN

class Socket;

class ServerWorkerThread: public Thread {
    public:
        ServerWorkerThread();
        virtual ~ServerWorkerThread();
        virtual int32_t prepareToRun();
        virtual std::string getThreadName();
        virtual std::string getEventName();
    protected:
        Queue<Socket> *works;
        virtual bool canBeBlocked();
        virtual void process();
};

DEF_NS_XCF_END

#endif /* XCF_SERVERWORKERTHREAD_H_ */
