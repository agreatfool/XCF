#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log(): privilege(LogPrivilege::Debug) {}

    Log::~Log() {}

    void Log::debug(std::string msg) const {
        this->log(LogPrivilege::Debug, msg);
    }

    void Log::info(std::string msg) const {
        this->log(LogPrivilege::Info, msg);
    }

    void Log::notice(std::string msg) const {
        this->log(LogPrivilege::Notice, msg);
    }

    void Log::warn(std::string msg) const {
        this->log(LogPrivilege::Warning, msg);
    }

    void Log::error(std::string msg) const {
        this->log(LogPrivilege::Error, msg);
    }

    void Log::setPrivilege(int privilege) {
        this->privilege = privilege;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SysLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SysLog::SysLog(): Log() {}

    SysLog::~SysLog() {}

    void SysLog::log(int privilege, std::string msg) const {
        if (privilege >= this->privilege) {
            std::cout << "SysLog: " << msg << std::endl;
        }
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* FileLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    FileLog::FileLog(): Log() {}

    FileLog::~FileLog() {}

    void FileLog::log(int privilege, std::string msg) const {
        if (privilege >= this->privilege) {
            std::cout << "FileLog: " << msg << std::endl;
        }
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* LogFactory
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log* LogFactory::instance = NULL;

    LogFactory::~LogFactory() {}

    Log* LogFactory::get() {
        if (LogFactory::instance == NULL) {
            LogFactory::instance = new SysLog();
        }
        return LogFactory::instance;
    }

    Log* LogFactory::get(int logType) {
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

} /* namespace XCF */
