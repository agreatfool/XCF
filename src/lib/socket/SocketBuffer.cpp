#include "../../Declare.h"
#include "SocketBuffer.h"

namespace XCF {

    SocketBuffer::SocketBuffer():
        buffer(new char[SOCK_BUFFER_LENGTH]()) {}

    SocketBuffer::SocketBuffer(char buff[]):
        buffer(new char[SOCK_BUFFER_LENGTH]())
    {
        this->copyBuffer(buff);
    }

    SocketBuffer::~SocketBuffer() {
        delete []this->buffer;
    }

} /* namespace XCF */
