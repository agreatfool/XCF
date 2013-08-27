#ifndef XCF_SOCKETBUFFER_H_
#define XCF_SOCKETBUFFER_H_

#include "../../Declare.h"
#include "../utility/Utility.h"

namespace XCF {

    class SocketBuffer {
        public:
            SocketBuffer();
            SocketBuffer(char buff[]);
            virtual ~SocketBuffer();
            inline char *getBuffer() {
                return this->buffer;
            };
            inline void copyBuffer(char buff[]) {
                Utility::appendCharArray(this->buffer, buff, SOCK_BUFFER_LENGTH - strlen(this->buffer) - 1);
            };
            inline void clearBuffer() {
                delete []this->buffer;
                this->buffer = new char[SOCK_BUFFER_LENGTH]();
            };
        protected:
            char *buffer;
    };

} /* namespace XCF */

#endif /* XCF_SOCKETBUFFER_H_ */
