#include "Time.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Time
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Time::~Time() {}

    time_t Time::getTimestamp() {
        return time(NULL);
    }

    struct tm *Time::getTimeStruct() {
        time_t currentTime = time(NULL);
        return Time::getTimeStruct(&currentTime);
    }

    struct tm *Time::getTimeStruct(time_t* time) {
        return localtime(time);
    }

    std::string Time::getTimeString() {
        time_t currentTime = time(NULL);
        return Time::getTimeString(&currentTime);
    }

    std::string Time::getTimeString(time_t* time) {
        char tmstr[32] = {0};
        strftime(tmstr, sizeof(tmstr), "%Y-%m-%d %H:%M:%S", Time::getTimeStruct(time));
        return Utility::charToString(tmstr);
    }

    std::string Time::getDefaultTimeString() {
        time_t currentTime = time(NULL);
        return Time::getDefaultTimeString(&currentTime);
    }

    std::string Time::getDefaultTimeString(time_t* time) {
        return Utility::charToString(ctime(time));
    }

} /* namespace XCF */
