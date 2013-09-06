#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log():
        priority(LogPriority::Debug),
        maxMsgCount(XCF_LOG_MSG_MAX_LIMIT),
        messages(new std::deque<std::string>()) {}

    Log::~Log() {}

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

                this->logToConsole(msg);
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
    LogFactory::~LogFactory() {}

    Log* LogFactory::get() {
        return LogFactory::get(LogType::SysLog);
    };

    Log* LogFactory::get(uint16_t logType) {
        Log* instance = NULL;
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
        return instance;
    };

} /* namespace XCF */
