#include "Utility.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Utility
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Utility::~Utility() {}

   std::string Utility::charToString(char *str) {
       std::string string = str;
       return string;
   }

    char *Utility::stringToChar(std::string str) {
        char *buff = new char[str.length() + 1];
        strcpy(buff, str.c_str());
        return buff;
    }

    std::string Utility::stringFormat(const std::string format, ...) {
        int size = 100;
        std::string str;
        va_list ap;
        while (1) {
            str.resize(size);
            va_start(ap, format);
            int n = vsnprintf((char *)str.c_str(), size, format.c_str(), ap);
            va_end(ap);
            if (n > -1 && n < size) {
                str.resize(n);
                return str;
            }
            if (n > -1) {
                size = n + 1;
            } else {
                size *= 2;
            }
        }
        return str;
    }

    void Utility::appendCharArray(char *appendBase, char *appendPart, int32_t length) {
        strncat(appendBase, appendPart, length);
    }

    bool Utility::isNullPtr(void *ptr) {
        if (NULL == ptr) {
            return true;
        } else {
            return false;
        }
    }

} /* namespace XCF */
