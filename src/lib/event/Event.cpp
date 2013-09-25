#include "Event.h"
#include "../thread/Thread.h"
#include "../thread/ThreadUtil.h"
#include "../log/Log.h"
#include "../utility/Utility.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//- Event
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
Event::Event():
    loop(
#if defined (__APPLE__)
        ev_loop_new(EVBACKEND_KQUEUE | EVFLAG_NOENV)
#elif defined (__linux)
        ev_loop_new(EVBACKEND_EPOLL | EVFLAG_NOENV)
#endif
    ), lock(ThreadUtil::createLock()) {}

Event::~Event() {
    this->stopLoop();
    ev_loop_destroy(this->loop);
}

void Event::startLoop() {
    LogFactory::get()->info(Utility::stringFormat("[%s] Loop started ...", this->getEventName().c_str()));
    ev_run(this->loop, 0);
}

void Event::stopLoop() {
    LogFactory::get()->info(Utility::stringFormat("[%s] Loop stopped ...", this->getEventName().c_str()));
    this->suspendLoop();
    this->clearWatchers();
    ev_break(this->loop, EVBREAK_ALL);
}

void Event::suspendLoop() {
    ev_suspend(this->loop);
}

void Event::resumeLoop() {
    ev_resume(this->loop);
}

DEF_NS_XCF_END
