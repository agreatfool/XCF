#ifndef XCF_UTILITY_H_
#define XCF_UTILITY_H_

#include <cstring>
#include <iostream>

namespace XCF {

    class Utility {
        public:
            Utility();
            virtual ~Utility();
            /**
             * Convert char* to std::string.
             */
            static std::string charToString(char *str);
            /**
             * Convert std::string to char*.
             * Since we cannot determine the length of the std::string at compiling time,
             * we have to use "new" to initialize a char* in HEAP with the length of given std::string.
             * DO NOT forget to using "delete []buff;" to free the memory.
             */
            static char *stringToChar(std::string str);
            /**
             * Format a string according to the "format".
             */
            static std::string stringFormat(const std::string format, ...);
            /**
             * Append "appendPart" to the end of "appendBase".
             */
            static void appendCharArray(char *appendBase, char *appendPart, int32_t length);
            /**
             * Check whether the "*ptr" is NULL pointer or not.
             */
            static bool isNullPtr(void *ptr);
    };

} /* namespace XCF */

#endif /* XCF_UTILITY_H_ */
