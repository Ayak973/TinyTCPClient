/* 
 * File:   socket.h
 * Author: ayak973
 *
 * Created on 1 décembre 2017, 22:14
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
            Socket(int fd);
            int getSocket() const;
            static constexpr int _fdInvalid = -1;
            
        public:
            virtual ~Socket();
            
            //can move
            Socket(Socket&& socket)                 noexcept;
            Socket& operator=(Socket&& socket)      noexcept;
            void swap(Socket& other)                noexcept;
            
            //cannot copy
            Socket(Socket const&)                   = delete;
            Socket& operator=(Socket const&)        = delete;
            
            void close();
    };
    
    class RWSocket : public Socket {
        private:
                       
        protected:
                        
        public:
            RWSocket(int fd);
        
            std::size_t getData(char* buffer, std::size_t maxLen);
            void putData(const char* buffer, std::size_t len);
    };
    
    class ConnectedSocket : public RWSocket {
        private:
        
        protected:
            
        public:
            ConnectedSocket(const std::string& host, const int port);
    };
}



#endif