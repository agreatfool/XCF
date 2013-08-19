#ifndef XCF_LOG_H_
#define XCF_LOG_H_

#include <iostream>
#include <syslog.h>

namespace XCF {

    namespace LogType {
        enum LogType {
            SysLog, FileLog
        };
    }

    namespace LogPrivilege {
        enum LogPrivilege {
            Debug, Info, Notice, Warning, Error
        };
    }

    class Log {
        public:
            Log();
            virtual ~Log();
            virtual void log(int privilege, std::string msg) const = 0;
            virtual void log(std::string msg) const = 0;
            void setPrivilege(int privilege);
        protected:
            int privilege; // FIXME
    };

    class SysLog: public Log {
        public:
            SysLog();
            virtual ~SysLog();
            void log(int privilege, std::string msg) const;
            void log(std::string msg) const;
    };

    class FileLog: public Log {
        public:
            FileLog();
            virtual ~FileLog();
            void log(int privilege, std::string msg) const;
            void log(std::string msg) const;
    };

    class LogFactory {
        public:
            virtual ~LogFactory();
            static Log* get(int logType);
            static Log* get();
        private:
            LogFactory();
            static Log* instance;
            // Stop the compiler generating methods of copy the object
            LogFactory(LogFactory const& copy);            // Not Implemented
            LogFactory& operator=(LogFactory const& copy); // Not Implemented
    };

} /* namespace XCF */

#endif /* XCF_LOG_H_ */
