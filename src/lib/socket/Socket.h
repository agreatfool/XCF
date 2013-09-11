#ifndef XCF_SOCKET_H_
#define XCF_SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "../../XCF.h"

DEF_NS_XCF_BEGIN

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
            char *getBuffer();
            /**
             * Copy & append char array to the end of "this->buffer".
             */
            void copyBuffer(char buff[]);
            /**
             * Clear "this->buffer".
             */
            void clearBuffer();
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
            int32_t  getSocketFd() const;
            /**
             * Get this->socketStatus.
             */
            uint16_t getSocketStatus() const;
            /**
             * Accept socket from "acceptFromFd".
             * If failed, NULL pointer returned.
             */
            static Socket *socketAccept(int32_t acceptFromFd, uint16_t protocol);
            /**
             * Release all the socket resources,
             * make it the same status as before "socketInit()" was called.
             */
            int32_t socketRelease();
            /**
             * Read bytes from socket.
             * With "SocketBuffer *buffer".
             */
            int32_t read(char *buffer, int32_t length);
            /**
             * Read bytes from socket.
             * With "SocketBuffer *buffer".
             */
            int32_t read(SocketBuffer *buffer, int32_t length);
            /**
             * Write bytes into socket.
             * With raw "char* buffer".
             */
            int32_t write(char *buffer);
            /**
             * Write bytes into socket.
             * With "SocketBuffer *buffer".
             */
            int32_t write(SocketBuffer *buffer);
            /**
             * Set socket write buffer size to "bufferSize".
             */
            int32_t setWriteBuffSize(int32_t bufferSize);
        protected:
            // Inputed
            std::string        socketHost;
            uint16_t           socketPort;
            uint16_t           socketProtocolType;
            uint16_t           socketEndType;
            // Status
            int16_t            socketStatus;
            // Generated
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
            int32_t setNonBlock();
            /**
             * Set socket to keep-alive.
             */
            int32_t setKeepAlive();
            /**
             * Set socket to reuse address.
             */
            int32_t setReuseAddr();
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketPool
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    typedef Map<int32_t, Socket>                  SocketPoolMap;
    typedef std::map<int32_t, Socket *>::iterator SocketPoolIterator;

    class SocketPool {
        public:
            SocketPool();
            virtual ~SocketPool();
            /**
             * Add socket into SocketPoolMap.
             */
            bool addSocket(Socket *socket);
            /**
             * Remove socket from SocketPoolMap.
             */
            bool removeSocket(int32_t socketFd);
            /**
             * Find & get socket from SocketPoolMap.
             * If specified Socket not found, NULL pointer returned.
             */
            Socket *getSocket(int32_t socketFd) const;
            /**
             * Remove all the Socket in the SocketPoolMap.
             */
            void clearSockets();
            /**
             * Get socket pool size.
             */
            uint32_t getPoolSize() const;
            /**
             * Find the Socket in the SocketPoolMap.
             * SocketPoolIterator returned.
             */
            SocketPoolIterator findSocket(int32_t socketFd) const;
        protected:
            SocketPoolMap *socketPool;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* inline Implementations
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketBuffer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    inline char *SocketBuffer::getBuffer() {
        return this->buffer;
    }
    inline void SocketBuffer::clearBuffer() {
        bzero(this->buffer, SOCK_BUFFER_LENGTH);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* Socket
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    inline int32_t Socket::getSocketFd() const {
        return this->socketFd;
    }
    inline uint16_t Socket::getSocketStatus() const {
        return this->socketStatus;
    }

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    //-* SocketPool
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    inline bool SocketPool::addSocket(Socket *socket) {
        return this->socketPool->add(socket->getSocketFd(), socket);
    }
    inline bool SocketPool::removeSocket(int32_t socketFd) {
        return this->socketPool->remove(socketFd);
    }
    inline Socket *SocketPool::getSocket(int32_t socketFd) const {
        return this->socketPool->get(socketFd);
    }
    inline void SocketPool::clearSockets() {
        this->socketPool->clear();
    }
    inline uint32_t SocketPool::getPoolSize() const {
        return this->socketPool->count();
    }
    inline SocketPoolIterator SocketPool::findSocket(int32_t socketFd) const {
        return this->socketPool->find(socketFd);
    }

DEF_NS_XCF_END

#endif /* XCF_SOCKET_H_ */
