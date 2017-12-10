#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "stringhelper.h"

using namespace tinytcp;

Socket::Socket(int fd) : _fd(fd) {
    if (_fd < 0) {
        throw std::runtime_error(str::createMultiStr("Socket::", __func__, ": invalid socket ", strerror(errno)));
    }
}

Socket::~Socket() {
    if (_fd != _fdInvalid) {
        try {
            this->close();
        }
        catch(...) { }
    }
}

int Socket::getSocket() const {
    return _fd;
}

void Socket::close() {
    if (_fd < 0) {
        throw std::logic_error(str::createMultiStr("Socket::", __func__, ": cannot close an invalid socket"));
    }
    
    int sockState = ::close(_fd);
    
    if (sockState != 0) {
        switch(errno) {
            case EBADF:
                throw std::runtime_error(str::createMultiStr("Socket::", __func__, " close error: fd isn't a valid open file descriptor"));
            case EINTR:
                throw std::runtime_error(str::createMultiStr("Socket::", __func__, " close error: the call was interrupted by a signal"));
            case EIO:
                throw std::runtime_error(str::createMultiStr("Socket::", __func__, " close error: an I/O error occurred"));
            default:
                throw std::runtime_error(str::createMultiStr("Socket::", __func__, " close error: unknown error"));
        }
    }
    
    _fd = _fdInvalid;
}

void Socket::swap(Socket& other) noexcept {
    std::swap(_fd, other._fd);
}

Socket::Socket(Socket&& socket) noexcept : _fd(_fdInvalid) {
    socket.swap(*this);
}

Socket& Socket::operator =(Socket&& socket) noexcept {
    socket.swap(*this);
    return *this;
}

RWSocket::RWSocket(int fd) : Socket(fd) { }
        
std::size_t RWSocket::getData(char* buffer, std::size_t maxLen) {
    if (this->getSocket() < 0) {
        throw std::logic_error(str::createMultiStr("RWSocket::", __func__, ": cannot get data from invalid socket"));
    }
    
    std::size_t totalRead = 0;
    ssize_t dataRead = 0;       //ssize_t != std::size_t : ssize_t is at least in range [-1, SSIZE_MAX]
    while(totalRead < maxLen) {
        dataRead = ::read(this->getSocket(), buffer + totalRead, maxLen - totalRead);
        if (dataRead == -1) {
            switch(errno) {
                case EBADF:
                case EFAULT:
                case EINVAL:
                case ENXIO:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": critical read error: ", strerror(errno)));
                case EIO:
                case ENOBUFS:
                case ENOMEM:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": read resource failure: ", strerror(errno)));
                case EINTR:
                case ETIMEDOUT:
                    continue;
                case ECONNRESET:
                case ENOTCONN:
                    dataRead = 0;
                    break;
                case EWOULDBLOCK:
                    //throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": read timeout"));
                    dataRead = 0;
                    break;
                default:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": read returned -1: ", strerror(errno)));
            }
        }
        if (dataRead == 0) {
            break;
        }
        totalRead += dataRead;
        
    }
    
    return totalRead;
}

void RWSocket::putData(const char* buffer, std::size_t len) {
    std::size_t totalWritten = 0;
    ssize_t dataWritten = 0;
    
    while (totalWritten < len) {
        dataWritten = ::write(this->getSocket(), buffer + totalWritten, len - totalWritten);
        if (dataWritten == -1) {
            switch(errno) {
                case EINVAL:
                case EBADF:
                case ECONNRESET:
                case ENXIO:
                case EPIPE:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": critical write error: ", strerror(errno)));
                case EDQUOT:
                case EFBIG:
                case EIO:
                case ENETDOWN:
                case ENETUNREACH:
                case ENOSPC:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": write resource failure: ", strerror(errno)));
                case EINTR:
                    continue;
                case EWOULDBLOCK:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": write timeout"));
                    break;
                default:
                    throw std::runtime_error(str::createMultiStr("RWSocket::", __func__, ": write returned -1: ", strerror(errno)));
                
            }
        }
        totalWritten += dataWritten;
    }
}

TimedRWSocket::TimedRWSocket(int fd, unsigned int readTimeout, unsigned int writeTimeout) : RWSocket(fd) {
    if (readTimeout > 0) {
        struct timeval readTimeval = { readTimeout, writeTimeout };
        if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, (timeval *)&readTimeval, sizeof(readTimeval)) < 0) 
            throw std::runtime_error(str::createMultiStr("TimedReadSocket::", __func__, ": setsockopt read timeout failed: ", strerror(errno)));
    }
    
    if (writeTimeout > 0) {
        struct timeval writeTimeval = { writeTimeout, writeTimeout };
        if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, (timeval *)&writeTimeval, sizeof(writeTimeval)) < 0) 
            throw std::runtime_error(str::createMultiStr("TimedReadSocket::", __func__, ": setsockopt write timeout failed: ", strerror(errno)));
    }
}

ConnectedSocket::ConnectedSocket(const std::string& host, const int port) : TimedRWSocket(::socket(PF_INET, SOCK_STREAM, 0), 5, 0) {
    struct sockaddr_in server = { 0 };
    
    server.sin_family       = AF_INET;
    server.sin_port         = htons(port);
    server.sin_addr.s_addr  = inet_addr(host.c_str());
    
    if (::connect(this->getSocket(), (struct sockaddr*) &server, sizeof(server)) != 0) {
        close();
        throw std::runtime_error(str::createMultiStr("ConnectedSocket::", __func__, ": connect failed: ", strerror(errno)));
    }
}


