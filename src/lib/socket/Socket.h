#ifndef XCF_SOCKET_H_
#define XCF_SOCKET_H_

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <ev.h>

#include "../../Declare.h"
#include "../log/Log.h"
#include "../event/Event.h"

namespace XCF {

    namespace SocketProtocol {
        enum SocketProtocol {
            TCP = IPPROTO_TCP,
            UDP = IPPROTO_UDP
        };
    }

    namespace SocketEndType {
        enum SocketEndType {
            CLIENT,
            SERVER
        };
    }

    namespace SocketStatus {
        enum SocketStatus {
            ERROR,      // socket error
            CLOSED,     // socket closed, init status
            OPENED,     // socket opened, address built
            CONNECTED   // socket connected, listen finished | connect finished
        };
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketBuffer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class SocketBuffer {
        public:
            SocketBuffer();
            SocketBuffer(char buff[]);
            virtual ~SocketBuffer();
            /**
             * Get "this->buffer".
             */
            inline char *getBuffer() {
                return this->buffer;
            };
            /**
             * Copy & append char array to the end of "this->buffer".
             */
            inline void copyBuffer(char buff[]) {
                Utility::appendCharArray(this->buffer, buff, SOCK_BUFFER_LENGTH - strlen(this->buffer) - 1);
            };
            /**
             * Clear "this->buffer".
             */
            inline void clearBuffer() {
                bzero(this->buffer, SOCK_BUFFER_LENGTH);
            };
        protected:
            char *buffer;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Socket
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Socket {
        public:
            /**
             * Socket built to bind & listen to a server address.
             * OR
             * Socket built to connect to a server.
             */
            Socket(std::string host, uint16_t port, uint16_t protocol, uint16_t endType);
            /**
             * Socket accepted at server end, already connected.
             */
            Socket(int32_t socketFd, struct sockaddr_in socketAddr, uint16_t protocol);
            virtual ~Socket();
            /**
             * Get this->socketFd.
             */
            inline int32_t  getSocketFd() const {
                return this->socketFd;
            };
            /**
             * Get this->socketStatus.
             */
            inline uint16_t getSocketStatus() const {
                return this->socketStatus;
            };
            /**
             * Accept socket from "acceptFromFd".
             * If failed, NULL pointer returned.
             */
            static inline Socket *socketAccept(int32_t acceptFromFd, uint16_t protocol) {
                struct sockaddr_in socketAddr;
                socklen_t socketLen = sizeof(socketAddr);
                int32_t socketFd = accept(acceptFromFd, (struct sockaddr *) &socketAddr, &socketLen);

                if (socketFd < 0) {
                    Log* logger = LogFactory::get();
                    logger->error(Utility::stringFormat("[Socket] accept socket failed: [%d] %s", errno, strerror(errno)));
                    delete logger;
                    return NULL;
                }

                return new Socket(socketFd, socketAddr, protocol);
            };
            /**
             * Release all the socket resources,
             * make it the same status as before "socketInit()" was called.
             */
            int32_t socketRelease();
            /**
             * Read bytes from socket.
             * With "SocketBuffer *buffer".
             */
            inline int32_t read(char *buffer, int32_t length) {
                int32_t received = recv(this->socketFd, buffer, length, 0);
                if (received < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] error in recv: [%d] %s", errno, strerror(errno)));
                }
                return received;
            };
            /**
             * Read bytes from socket.
             * With "SocketBuffer *buffer".
             */
            inline int32_t read(SocketBuffer *buffer, int32_t length) {
                int32_t received = recv(this->socketFd, buffer->getBuffer(), length, 0);
                if (received < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] error in recv: [%d] %s", errno, strerror(errno)));
                }
                return received;
            };
            /**
             * Write bytes into socket.
             * With raw "char* buffer".
             */
            inline int32_t write(char *buffer) {
                int32_t transmitted = send(this->socketFd, buffer, strlen(buffer), 0);
                if (transmitted < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] error in send: [%d] %s", errno, strerror(errno)));
                }
                return transmitted;
            };
            /**
             * Write bytes into socket.
             * With "SocketBuffer *buffer".
             */
            inline int32_t write(SocketBuffer *buffer) {
                int32_t transmitted = send(this->socketFd, buffer->getBuffer(), strlen(buffer->getBuffer()), 0);
                if (transmitted < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] error in send: [%d] %s", errno, strerror(errno)));
                }
                return transmitted;
            };
            /**
             * Set socket write buffer size to "bufferSize".
             */
            inline int32_t setWriteBuffSize(int32_t bufferSize) {
                int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_SNDBUF, (const void *) &bufferSize, sizeof(socklen_t));
                if (setResult < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] setWriteBuffSize failed: [%d] %s", errno, strerror(errno)));
                }
                return setResult;
            }
        protected:
            // Inputed
            std::string        socketHost;
            uint16_t           socketPort;
            uint16_t           socketProtocolType;
            uint16_t           socketEndType;
            // Status
            int16_t            socketStatus;
            // Generated
            Log                *logger;
            int32_t            socketFd;
            struct sockaddr_in socketAddr;
            /**
             * Initialize the socket.
             * Server: socket() -> bind() -> listen()
             * Client: socket() -> connect()
             */
            int32_t socketInit();
            /**
             * Set socket to non-block.
             */
            inline int32_t setNonBlock() {
                int32_t flags = fcntl(this->socketFd, F_GETFL);
                int32_t setResult = fcntl(this->socketFd, F_SETFL, flags | O_NONBLOCK);
                if (setResult < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] setNonBlock failed: [%d] %s", errno, strerror(errno)));
                }
                return setResult;
            };
            /**
             * Set socket to keep-alive.
             */
            inline int32_t setKeepAlive() {
                int32_t flags = 1;
                int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(int32_t));
                if (setResult < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] setKeepAlive failed: [%d] %s", errno, strerror(errno)));
                }
                return setResult;
            };
            /**
             * Set socket to reuse address.
             */
            inline int32_t setReuseAddr() {
                int32_t flags = 1;
                int32_t setResult = setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int32_t));
                if (setResult < 0) {
                    this->logger->error(Utility::stringFormat("[Socket] setReuseAddr failed: [%d] %s", errno, strerror(errno)));
                }
                return setResult;
            };
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketPool
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    typedef std::map<int32_t, Socket *>           SocketPoolMap;
    typedef std::map<int32_t, Socket *>::iterator SocketPoolIterator;

    class SocketPool {
        public:
            SocketPool();
            virtual ~SocketPool();
            /**
             * Add socket into SocketPoolMap.
             */
            void addSocket(Socket *socket);
            /**
             * Remove socket from SocketPoolMap.
             */
            void removeSocket(int32_t socketFd);
            /**
             * Find & get socket from SocketPoolMap.
             * If specified Socket not found, NULL pointer returned.
             */
            Socket *getSocket(int32_t socketFd);
            /**
             * Remove all the Socket in the SocketPoolMap.
             */
            void clearSockets();
            /**
             * Get socket pool size.
             */
            inline int32_t getPoolSize() {
                return this->socketPool->size();
            };
            /**
             * Find the Socket in the SocketPoolMap.
             * SocketPoolIterator returned.
             */
            inline SocketPoolIterator findSocket(int32_t socketFd) {
                return this->socketPool->find(socketFd);
            };
        protected:
            SocketPoolMap *socketPool;
    };

} /* namespace XCF */

#endif /* XCF_SOCKET_H_ */
