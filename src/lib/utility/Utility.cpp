#include "Utility.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Utility
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Utility::~Utility() {}

    char* Utility::strToChar(std::string str) {
        char *buff = new char[str.length() + 1];
        strcpy(buff, str.c_str());

        return buff;
    }

} /* namespace XCF */
