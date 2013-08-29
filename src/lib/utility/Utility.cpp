#include "Utility.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Utility
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Utility::~Utility() {}

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

} /* namespace XCF */
