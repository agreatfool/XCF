#ifndef XCF_UTILITY_H_
#define XCF_UTILITY_H_

#include "../../Common.h"

#include <cstring>

#include <sys/param.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

DEF_NS_XCF_BEGIN

    class Utility {
        public:
            Utility();
            virtual ~Utility();
            /**
             * Convert char* to std::string.
             */
            static std::string charToString(const char *str);
            /**
             * Convert std::string to char*.
             * Since we cannot determine the length of the std::string at compiling time,
             * we have to use "new" to initialize a char* in HEAP with the length of given std::string.
             * DO NOT forget to using "delete []buff;" to free the memory.
             */
            static char *stringToChar(const std::string str);
            /**
             * Format a string according to the "format".
             */
            static std::string stringFormat(const std::string format, ...);
            /**
             * Append "appendPart" to the end of "appendBase".
             */
            static void appendCharArray(char *appendBase, const char *appendPart, int32_t length);
            /**
             * Check whether the "*ptr" is NULL pointer or not.
             */
            static bool isNullPtr(const void *ptr);
            /**
             * Get the CPU number of current machine.
             */
            static uint16_t getCpuNum();
            /**
             * Remove the tailing "\n" if exists.
             * This function will change the original "char *".
             */
            static void stringTrimLineBreak(char *str);
    };

DEF_NS_XCF_END

#endif /* XCF_UTILITY_H_ */
