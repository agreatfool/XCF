#ifndef XCF_TIME_H_
#define XCF_TIME_H_

#include <stdlib.h>
#include <ctime>

#include "Utility.h"

namespace XCF {

    class Time {
        public:
            Time();
            virtual ~Time();
            /**
             * Get current timestamp.
             */
            static inline time_t getTimestamp() {
                return time(NULL);
            };
            /**
             * Get struct tm from time_t.
             */
            static inline struct tm *getTimeStruct() {
                time_t currentTime = time(NULL);
                return Time::getTimeStruct(&currentTime);
            };
            static inline struct tm *getTimeStruct(time_t* time) {
                return localtime(time);
            };
            /**
             * Get time string from time_t as a std::string.
             * The format is "YYYY-mm-dd HH:MM:SS/0".
             */
            static inline std::string getTimeString() {
                time_t currentTime = time(NULL);
                return Time::getTimeString(&currentTime);
            };
            static inline std::string getTimeString(time_t* time) {
                char tmstr[32] = {0};
                strftime(tmstr, sizeof(tmstr), "%Y-%m-%d %H:%M:%S", Time::getTimeStruct(time));
                return Utility::charToString(tmstr);
            };
            /**
             * Get time string from time_t as a char*.
             * The format is "Wed Jan 02 02:03:55 1980/n/0".
             */
            static inline std::string getDefaultTimeString() {
                time_t currentTime = time(NULL);
                return Time::getDefaultTimeString(&currentTime);
            };
            static inline std::string getDefaultTimeString(time_t* time) {
                return Utility::charToString(ctime(time));
            };
    };

} /* namespace XCF */

#endif /* XCF_TIME_H_ */
