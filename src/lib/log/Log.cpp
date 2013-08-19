#include "Log.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Log::Log() {
        this->privilege = LogPrivilege::Debug;
    }

    Log::~Log() {}

    void Log::setPrivilege(int privilege) {
        this->privilege = privilege;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SysLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    SysLog::SysLog(): Log() {}

    SysLog::~SysLog() {}

    void SysLog::log(int privilege, std::string msg) {
        if (this->privilege >= privilege) {
            std::cout << "SysLog: " << msg << std::endl;
        }
    }

    void SysLog::log(std::string msg) {
        this->log(LogPrivilege::Debug, msg);
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* FileLog
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    FileLog::FileLog(): Log() {}

    FileLog::~FileLog() {}

    void FileLog::log(int privilege, std::string msg) {
        if (this->privilege >= privilege) {
            std::cout << "FileLog: " << msg << std::endl;
        }
    }

    void FileLog::log(std::string msg) {
        this->log(LogPrivilege::Debug, msg);
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
