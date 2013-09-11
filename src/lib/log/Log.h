#ifndef XCF_LOG_H_
#define XCF_LOG_H_

#include "../../Common.h"

#include <deque>
#include <syslog.h>

DEF_NS_XCF_BEGIN

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
        /**
         * Log debug message.
         */
        void debug(std::string msg)  const;
        /**
         * Log info message.
         */
        void info(std::string msg)   const;
        /**
         * Log notice message.
         */
        void notice(std::string msg) const;
        /**
         * Log warning message.
         */
        void warn(std::string msg)   const;
        /**
         * Log error message.
         */
        void error(std::string msg)  const;
        /**
         * Set log priority.
         */
        void setPriority(uint16_t priority);
        /**
         * Register periodic event to output logs.
         */
        void registerTimer();
        /**
         * Static periodic event callback function, just output the logs.
         */
        static void timerCallback(EventLoop *loop, EventPeriodicWatcher *watcher, int32_t revents);
    protected:
        uint16_t priority;
        /**
         * How many logs would be cached before output.
         */
        uint16_t maxMsgCount;
        std::deque<std::string> *messages;
        /**
         * Cache message into the queue, wait for the output.
         */
        void cacheMessage(uint16_t priority, std::string msg) const;
        /**
         * Log message into std::cout.
         */
        void logToConsole(std::string msg) const;
        /**
         * Output functionality to be implemented by child classes.
         */
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
        /**
         * Get Log instance according to "logType".
         */
        static Log *get(uint16_t logType);
        /**
         * Get Log instance.
         */
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
