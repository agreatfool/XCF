#ifndef XCF_LOG_H_
#define XCF_LOG_H_

#include <iostream>
#include <deque>
#include <syslog.h>

namespace XCF {

    namespace LogType {
        enum LogType {
            SysLog, FileLog
        };
    }

    namespace LogPriority {
        enum LogPriority {
            Debug   = LOG_DEBUG,
            Info    = LOG_INFO,
            Notice  = LOG_NOTICE,
            Warning = LOG_WARNING,
            Error   = LOG_ERR
        };
    }

    class Log {
        public:
            Log();
            virtual ~Log();
            void debug(std::string msg)  const;
            void info(std::string msg)   const;
            void notice(std::string msg) const;
            void warn(std::string msg)   const;
            void error(std::string msg)  const;
            void setPriority(uint16_t priority);
        protected:
            uint16_t priority;
            virtual void output(uint16_t priority, std::string msg) const = 0;
            void inline logToConsole(std::string msg) const;
    };

    class SysLog: public Log {
        public:
            SysLog();
            virtual ~SysLog();
            void output(uint16_t priority, std::string msg) const;
    };

    class FileLog: public Log {
        public:
            FileLog();
            virtual ~FileLog();
            void output(uint16_t priority, std::string msg) const;
    };

    class LogFactory {
        public:
            virtual ~LogFactory();
            static Log* get();
            static Log* get(uint16_t logType);
        private:
            LogFactory();
            // Stop the compiler generating methods of copy the object
            LogFactory(LogFactory const& copy);            // Not Implemented
            LogFactory& operator=(LogFactory const& copy); // Not Implemented
    };

} /* namespace XCF */

#endif /* XCF_LOG_H_ */
