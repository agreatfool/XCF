#ifndef XCF_UTILITY_H_
#define XCF_UTILITY_H_

#include <iostream>

namespace XCF {

    class Utility {
        public:
            Utility();
            virtual ~Utility();
            static char* strToChar(std::string str);
    };

} /* namespace XCF */

#endif /* UTILITY_H_ */
