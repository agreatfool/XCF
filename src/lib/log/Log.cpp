#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log():
        maxMsgCount(XCF_LOG_MSG_MAX_LIMIT),
        priority(LogPriority::Debug),
        messages(new std::deque<std::string>()) {}

    Log::~Log() {}

    void inline Log::cacheMessage(uint16_t priority, std::string msg) const {
        if (priority <= this->priority) {
            this->messages->push_back(msg);
            if (this->messages->size() >= this->maxMsgCount) {
                this->output();
            }
        }
    }

    void inline Log::logToConsole(std::string msg) const {
        std::cout << msg << std::endl;
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

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SysLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SysLog::SysLog(): Log() {}

    SysLog::~SysLog() {}

    void SysLog::output() const {
        if (this->messages->size() > 0) {
            openlog("XCF", LOG_PID, LOG_USER);

            while (!this->messages->empty()) {
                std::string msg = this->messages->front();
                const char *buff = msg.c_str();

                syslog(LOG_USER | this->priority, "%s", buff);

                delete []buff;

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

    FileLog::~FileLog() {}

    void FileLog::output() const {
        // TODO tobe implemented
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* LogFactory
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log* LogFactory::instance = NULL;

    LogFactory::~LogFactory() {}

    Log* LogFactory::get() {
        return LogFactory::get(LogType::SysLog);
    }

    Log* LogFactory::get(uint16_t logType) {
        if (LogFactory::instance == NULL) {
            switch (logType) {
                case LogType::SysLog:
                    LogFactory::instance = new SysLog();
                    break;
                case LogType::FileLog:
                    LogFactory::instance = new FileLog();
                    break;
                default:
                    LogFactory::instance = new SysLog();
                    break;
            }
        }
        return LogFactory::instance;
    }

    void LogFactory::reset() {
        delete LogFactory::instance;
        LogFactory::instance = NULL;
    }

} /* namespace XCF */
