#ifndef XCF_LOG_H_
#define XCF_LOG_H_

#include <iostream>
#include <deque>
#include <syslog.h>

namespace XCF {

    #define XCF_LOG_MSG_MAX_LIMIT 25

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
            void setPriority(uint16_t privilege);
            virtual void output() const = 0;
        protected:
            uint16_t                maxMsgCount; // see XCF_LOG_MSG_MAX_LIMIT
            uint16_t                priority;
            std::deque<std::string> *messages;
            void inline cacheMessage(uint16_t priority, std::string msg) const;
            void inline logToConsole(std::string msg) const;
    };

    class SysLog: public Log {
        public:
            SysLog();
            virtual ~SysLog();
            void output() const;
    };

    class FileLog: public Log {
        public:
            FileLog();
            virtual ~FileLog();
            void output() const;
    };

    class LogFactory {
        public:
            virtual ~LogFactory();
            static Log* get(uint16_t logType);
            static Log* get();
            static void reset();
        private:
            LogFactory();
            static Log* instance;
            // Stop the compiler generating methods of copy the object
            LogFactory(LogFactory const& copy);            // Not Implemented
            LogFactory& operator=(LogFactory const& copy); // Not Implemented
    };

} /* namespace XCF */

#endif /* XCF_LOG_H_ */
