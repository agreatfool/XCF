#include "Log.h"
#include "../utility/Time.h"
#include "../utility/Utility.h"
#include "../event/Event.h"
#include "../event/app/Timer.h"
#include "../thread/ThreadUtil.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* Log
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
Log::Log():
    priority(LogPriority::Debug),
    maxMsgCount(XCF_LOG_MSG_MAX_LIMIT),
    messages(new Deque<std::string>()) {}

Log::~Log() {}

void Log::registerTimer() {
    Timer::get()->addWatcher(XCF_LOG_TIMER_NAME, Log::timerCallback, XCF_LOG_TIMER_INTERVAL);
}

void Log::timerCallback(EventLoop *loop, EventPeriodicWatcher *watcher, int32_t revents) {
    LogFactory::get()->output();
}

void Log::cacheMessage(uint16_t priority, std::string msg) const {
    if (priority <= this->priority) {
        std::string *formatted = new std::string(Utility::stringFormat("[%s] %s", Time::getTimeString().c_str(), msg.c_str()));
        this->messages->pushBack(formatted); // self made Deque requres a pointer
        this->logToConsole(*formatted);
        if (this->messages->count() >= this->maxMsgCount) {
            this->output();
        }
    }
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* SysLog
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
SysLog::SysLog(): Log() {}

SysLog::~SysLog() {
    this->output();
}

void SysLog::output() const {
    if (!this->messages->empty()) {
        openlog("XCF", LOG_PID, LOG_USER);
        while (!this->messages->empty()) {
            std::string *msg = this->messages->popFront();
            syslog(LOG_USER | this->priority, "%s", msg->c_str());
            delete msg;
        }
        closelog();
    }
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* FileLog
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
FileLog::FileLog(): Log() {}

FileLog::~FileLog() {
    this->output();
}

void FileLog::output() const {
    // TODO tobe implemented
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* LogFactory
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
Log *LogFactory::instance = NULL;
ThreadLock LogFactory::lock = PTHREAD_MUTEX_INITIALIZER;

LogFactory::~LogFactory() {}

Log* LogFactory::get() {
    return LogFactory::get(LogType::SysLog);
};

Log *LogFactory::get(uint16_t logType) {
    if (LogFactory::instance == NULL) {
        ThreadUtil::lock(&LogFactory::lock);
        switch (logType) {
            case LogType::SysLog:
                instance = new SysLog();
                break;
            case LogType::FileLog:
                instance = new FileLog();
                break;
            default:
                instance = new SysLog();
                break;
        }
        ThreadUtil::unlock(&LogFactory::lock);
    }
    return LogFactory::instance;
};

DEF_NS_XCF_END
