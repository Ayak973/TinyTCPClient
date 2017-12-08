/* 
 * File:   main.cpp
 * Author: Ayak973
 *
 * Created on December 5, 2017, 19:01
 * 
 * Inspired by https://github.com/Loki-Astari/Examples/blob/master/Version2/
 * 
 */

#include <iostream>
#include <string>
#include <array>
#include <thread>
#include <mutex>

#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <netdb.h>	//hostent
#include <unistd.h>

//#include "socket.h"
#include "stringhelper.h"
#include "socket.h"

using std::cout;
using std::endl;

static const unsigned int parallelJobsCount = 40;
static const int port = 80;
static const std::string host = "192.168.16.237";
static const std::string sendStr = "GET /index.html HTTP/1.1\r\nHost: notknownhost\r\nUser-Agent: TinyTCPClient alpha\r\n\r\n";

//std::cout synchronization
std::mutex printMtx;

void sendAndReceive(unsigned int threadId);
//int createSocketAndConnect(std::string& error);
//int sendAll(int s, const char* buf, int &len);
void blockingPrint(const std::string& str);

int main(int argc, char** argv) {
    cout << "Starting client..." << endl;

    //client parallel jobs array
    std::array<std::thread, parallelJobsCount> threadArray;
    
    //starting threads
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        threadArray[i] = std::thread(sendAndReceive, i);
    }

    //waiting for threads to finish
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        if (threadArray[i].joinable()) threadArray[i].join();
    }
    
    cout << "Clients finished !" << endl;
    
    return 0;
}

void sendAndReceive(unsigned int threadId) {
    blockingPrint("Starting thread #" + std::to_string(threadId) + "...");
    
    tinytcp::ConnectedSocket connect(host, port);
    
    static const int bufferMaxLen = 4096;
    std::array<char, bufferMaxLen> buffer = { 0 };
    
    connect.putData(sendStr.c_str(), sendStr.size());
    connect.getData(&buffer[0], bufferMaxLen);
    
    blockingPrint(std::string(&buffer[0], bufferMaxLen));
    
//    std::string error;
//    int clientfd = -1;
//    
//    clientfd = createSocketAndConnect(error);
//    if (clientfd < 0) {
//        blockingPrint("Thread #" + std::to_string(threadId) + " error: " + error + " !");
//        return;
//    }
//    
//    int sendLen = sendStr.length();
//    
//    if (sendAll(clientfd, sendStr.c_str(), sendLen) == -1) {
//        blockingPrint("Thread #" + std::to_string(threadId) + " cannot sendAll !");
//        return;
//    }
//    
//    
//    if (clientfd > -1) close(clientfd);
    
    blockingPrint("Thread #" + std::to_string(threadId) + " stopped !");
}

//int createSocketAndConnect(std::string& error) {
//    int fd = ::socket(AF_INET , SOCK_STREAM , 0);
//    
//    if (fd < 0) {
//        error = "cannot create socket";
//        return -1;
//    }
//    
//    struct sockaddr_in serverAddr = { 0 };
//    serverAddr.sin_family       = AF_INET;
//    serverAddr.sin_port         = htons(port);
//    serverAddr.sin_addr.s_addr  = inet_addr(host.c_str());
//    
//    if (::connect(fd, (struct sockaddr*) &serverAddr, sizeof(struct sockaddr)) == -1) {
//        ::close(fd);
//        error = "connect() failed";
//        return -1;        
//    }
//    
//    return fd;
//}

void blockingPrint(const std::string& str) {
    std::lock_guard<std::mutex> lck(printMtx);
    cout << str << endl;
}

//int sendAll(int sock, const char* buf, int &len) {
//    int total = 0;
//    int bytesleft = len;
//    int n;
//    
//    while (total < len && bytesleft > 0) {
//        n = ::send(sock, buf + total, bytesleft, 0);
//        if (n == -1) { break; }
//        total += n;
//        bytesleft -= n;
//    }
//    
//    len = total;
//    
//    return (n == -1) ? -1 : 0;
//}
