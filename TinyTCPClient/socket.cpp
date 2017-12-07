#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>


using namespace tinytcp;

Socket::Socket(int type, int domain, int protocol) : _fd(-1) {
    _fd = ::socket(type, domain, protocol);
    if (_fd == -1) {
        
    }
}

int Socket::getSocket() const {
    return _fd;
}

