#include "socket.h"

#include <sys/socket.h>

using namespace tinytcp;

Socket::Socket(int type, int domain, int protocol) : _fd(-1) {
    ::socket(type, domain, protocol);
    
}

int Socket::getSocket() const {
    return _fd;
}

