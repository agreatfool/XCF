#include "Log.h"
#include "../utility/Time.h"
#include "../utility/Utility.h"
#include "../event/Event.h"
#include "../event/Timer.h"

DEF_NS_XCF_BEGIN

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//-* Log
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
Log::Log():
    priority(LogPriority::Debug),
    maxMsgCount(XCF_LOG_MSG_MAX_LIMIT),
    messages(new std::deque<std::string>()) {}

Log::~Log() {}

void Log::registerTimer() {
    Timer::get()->addWatcher(XCF_LOG_TIMER_NAME, Log::timerCallback, XCF_LOG_TIMER_INTERVAL);
}

void Log::debug(std::string msg) const {
    this->cacheMessage(LogPriority::Debug, msg);
}

void Log::info(std::string msg) const {
    this->cacheMessage(LogPriority::Info, msg);
}

void Log::notice(std::string msg) const {
    this->cacheMessage(LogPriority::Notice, msg);
}

void Log::warn(std::string msg) const {
    this->cacheMessage(LogPriority::Warning, msg);
}

void Log::error(std::string msg) const {
    this->cacheMessage(LogPriority::Error, msg);
}

void Log::setPriority(uint16_t priority) {
    this->priority = priority;
}

void Log::timerCallback(EventLoop *loop, EventPeriodicWatcher *watcher, int32_t revents) {
    LogFactory::get()->output();
}

void Log::cacheMessage(uint16_t priority, std::string msg) const {
    if (priority <= this->priority) {
        std::string formatted = Utility::stringFormat("[%s] %s", Time::getTimeString().c_str(), msg.c_str());
        this->messages->push_back(formatted.c_str());
        this->logToConsole(formatted);
        if (this->messages->size() >= this->maxMsgCount) {
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
    if (this->messages->size() > 0) {
        openlog("XCF", LOG_PID, LOG_USER);
        while (!this->messages->empty()) {
            std::string msg = this->messages->front();
            const char *buff = msg.c_str();

            syslog(LOG_USER | this->priority, "%s", buff);

            this->messages->pop_front();
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

LogFactory::~LogFactory() {}

Log* LogFactory::get() {
    return LogFactory::get(LogType::SysLog);
};

Log *LogFactory::get(uint16_t logType) {
    if (LogFactory::instance == NULL) {
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
    }
    return LogFactory::instance;
};

DEF_NS_XCF_END
