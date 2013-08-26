#ifndef XCF_UTILITY_H_
#define XCF_UTILITY_H_

#include <iostream>

namespace XCF {

    class Utility {
        public:
            Utility();
            virtual ~Utility();
            /**
             * Convert char* to std::string.
             */
            static inline std::string charToString(char* str) {
                std::string string = str;
                return string;
            };
            /**
             * Convert std::string to char*.
             * Since we cannot determine the length of the std::string at compiling time,
             * we have to use "new" to initialize a char* in HEAP with the length of given std::string.
             * DO NOT forget to using "delete []buff;" to free the memory.
             */
            static inline char* stringToChar(std::string str) {
                char* buff = new char[str.length() + 1];
                strcpy(buff, str.c_str());
                return buff;
            };
    };

} /* namespace XCF */

#endif /* XCF_UTILITY_H_ */
