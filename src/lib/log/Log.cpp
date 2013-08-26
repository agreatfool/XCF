#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log(): priority(LogPriority::Debug) {}

    Log::~Log() {}

    void inline Log::logToConsole(std::string msg) const {
        std::cout << msg << std::endl;
    }

    void Log::debug(std::string msg) const {
        this->output(LogPriority::Debug, msg);
    }

    void Log::info(std::string msg) const {
        this->output(LogPriority::Info, msg);
    }

    void Log::notice(std::string msg) const {
        this->output(LogPriority::Notice, msg);
    }

    void Log::warn(std::string msg) const {
        this->output(LogPriority::Warning, msg);
    }

    void Log::error(std::string msg) const {
        this->output(LogPriority::Error, msg);
    }

    void Log::setPriority(uint16_t priority) {
        this->priority = priority;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SysLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SysLog::SysLog(): Log() {}

    SysLog::~SysLog() {}

    void SysLog::output(uint16_t priority, std::string msg) const {
        if (priority <= this->priority) {
            openlog("XCF", LOG_PID, LOG_USER);

            const char* buff = msg.c_str();
            syslog(LOG_USER | this->priority, "%s", buff);
            this->logToConsole(msg);

            closelog();
        }
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* FileLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    FileLog::FileLog(): Log() {}

    FileLog::~FileLog() {}

    void FileLog::output(uint16_t priority, std::string msg) const {
        // TODO tobe implemented
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* LogFactory
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    LogFactory::~LogFactory() {}

    Log* LogFactory::get() {
        return LogFactory::get(LogType::SysLog);
    }

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
    }

} /* namespace XCF */
