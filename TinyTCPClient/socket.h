/* 
 * File:   nethelper.h
 * Author: ayak973
 *
 * Created on 23 juillet 2017, 22:14
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <iostream>

namespace tinytcp {
    
    class Socket {
        private:
            int _fd;
            
        protected:
            Socket(int sockfd);
            int getSocket() const { return _fd; }
            
        public:
            ~Socket();
            
            //can move
            Socket(Socket&& socket)                 noexcept;
            Socket& operator=(Socket&& socket)      noexcept;
            void swap(Socket& other)                noexcept;
            
            //cannot copy
            Socket(Socket const&)               = delete;
            Socket& operator=(Socket const&)    = delete;
            
            void close();
    };
}



#endif