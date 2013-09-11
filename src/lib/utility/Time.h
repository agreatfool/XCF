#ifndef XCF_TIME_H_
#define XCF_TIME_H_

#include <stdlib.h>
#include <ctime>

#include "../../Common.h"

DEF_NS_XCF_BEGIN

    class Time {
        public:
            Time();
            virtual ~Time();
            /**
             * Get current timestamp.
             */
            static time_t getTimestamp();
            /**
             * Get struct tm from time_t.
             */
            static struct tm *getTimeStruct();
            static struct tm *getTimeStruct(time_t* time);
            /**
             * Get time string from time_t as a std::string.
             * The format is "YYYY-mm-dd HH:MM:SS/0".
             */
            static std::string getTimeString();
            static std::string getTimeString(time_t* time);
            /**
             * Get time string from time_t as a char*.
             * The format is "Wed Jan 02 02:03:55 1980/n/0".
             */
            static std::string getDefaultTimeString();
            static std::string getDefaultTimeString(time_t* time);
    };

DEF_NS_XCF_END

#endif /* XCF_TIME_H_ */
