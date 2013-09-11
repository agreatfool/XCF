#ifndef XCF_LOG_H_
#define XCF_LOG_H_

#include "../../Common.h"

#include <deque>
#include <syslog.h>

DEF_NS_XCF_BEGIN

    #define XCF_LOG_MSG_MAX_LIMIT  25
    #define XCF_LOG_TIMER_INTERVAL 5.
    #define XCF_LOG_TIMER_NAME     "XCF_LOG_TIMER"

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

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
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
            void registerTimer();
            static void timerCallback(EventLoop *loop, EventPeriodicWatcher *watcher, int32_t revents);
        protected:
            uint16_t priority;
            uint16_t maxMsgCount;
            std::deque<std::string> *messages;
            void cacheMessage(uint16_t priority, std::string msg) const;
            void logToConsole(std::string msg) const;
            virtual void output() const = 0;
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

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- LogFactory
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class LogFactory {
        public:
            virtual ~LogFactory();
            static Log *get(uint16_t logType);
            static Log *get();
        private:
            LogFactory();
            static Log        *instance;
            // Stop the compiler generating methods of copy the object
            LogFactory(LogFactory const& copy);            // Not Implemented
            LogFactory& operator=(LogFactory const& copy); // Not Implemented
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- inline Implementations
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //- Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    inline void Log::logToConsole(std::string msg) const {
        std::cout << msg << std::endl;
    }

DEF_NS_XCF_END

#endif /* XCF_LOG_H_ */
