#ifndef XCF_SERVERREADERTHREAD_H_
#define XCF_SERVERREADERTHREAD_H_

#include "../../../../Common.h"
#include "../../../thread/Thread.h"
#include "../../../event/ev/EventIo.h"

DEF_NS_XCF_BEGIN

class ServerReaderThread: public Thread, public EventIo {
    public:
        ServerReaderThread();
        virtual ~ServerReaderThread();
        virtual int32_t prepareToRun();
        virtual void run();
        static void readCallback(EventLoop *readLoop, EventIoWatcher *readWatcher, int revents);
    protected:
        virtual bool canBeBlocked();
        virtual void process();
};

DEF_NS_XCF_END

#endif /* XCF_SERVERREADERTHREAD_H_ */
