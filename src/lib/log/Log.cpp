#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log(): priority(LogPriority::Debug) {}

    Log::~Log() {}

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

            std::string formatted = Utility::stringFormat("[%s] %s", Time::getTimeString().c_str(), msg.c_str());
            const char* buff = formatted.c_str();
            syslog(LOG_USER | this->priority, "%s", buff);
            this->logToConsole(formatted);

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

} /* namespace XCF */
