#include "Utility.h"

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Utility
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    Utility::~Utility() {}

   std::string Utility::charToString(const char *str) {
       std::string string = str;
       return string;
   }

    char *Utility::stringToChar(const std::string str) {
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

    void Utility::appendCharArray(char *appendBase, const char *appendPart, int32_t length) {
        strncat(appendBase, appendPart, length);
    }

    bool Utility::isNullPtr(const void *ptr) {
        if (NULL == ptr) {
            return true;
        } else {
            return false;
        }
    }

    uint16_t Utility::getCpuNum() {
#if defined (__APPLE__)
        uint16_t cpuNum;
        int mib[4];
        size_t len = 4;

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl(mib, 2, &cpuNum, &len, NULL, 0);

        if (cpuNum < 1) {
             mib[1] = HW_NCPU;
             sysctl( mib, 2, &cpuNum, &len, NULL, 0 );

             if (cpuNum < 1) {
                  cpuNum = 1;
             }
        }
        return cpuNum;
#elif defined (__linux)
        return sysconf(_SC_NPROCESSORS_ONLN);
#endif
        return 1;
    }

} /* namespace XCF */
