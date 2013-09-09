#ifndef XCF_DECLARE_H_
#define XCF_DECLARE_H_

#include <ev.h>
#include <pthread.h>

namespace XCF {

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* constants
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    #define INIT_SOCKET_FD            0
    #define INVALID_SOCKET_FD        -1

    #define VALID_RESULT              0
    #define INVALID_RESULT           -1

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* socket
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    #define SOCK_LISTEN_BACKLOG 16
    #define SOCK_BUFFER_LENGTH  1024 * 16

} /* namespace XCF */

#endif /* XCF_DECLARE_H_ */
