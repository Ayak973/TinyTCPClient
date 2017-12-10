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

static const unsigned int parallelJobsCount = 4;
//string writed on socket
static const std::string sendStr = "GET / HTTP/1.1\r\nHost: notknownhost\r\nUser-Agent: TinyTCPClient alpha\r\n\r\n";

//std::cout synchronization
std::mutex printMtx;

void sendAndReceive(unsigned int threadId, std::string host, int port);
void blockingPrint(const std::string& str);

int main(int argc, char** argv) {
#ifndef DEBUG
    if (argc < 3) {
        cout << "usage: ./tinytcpclient host port" << endl;
        return -1;
    }
    
    std::string host(argv[1]);
    int port = std::stoi(argv[2]);
#else
    std::string host = "192.168.31.254";
    int port = 80;
#endif
    
    cout << "Starting client..." << endl;

    //client parallel jobs array
    std::array<std::thread, parallelJobsCount> threadArray;
    
    //starting threads
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        threadArray[i] = std::thread(sendAndReceive, i, host, port);
    }

    //waiting for threads to finish
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        if (threadArray[i].joinable()) threadArray[i].join();
    }
    
    cout << "Clients finished !" << endl;
    
    return 0;
}

void sendAndReceive(unsigned int threadId, std::string host, int port) {
    try {
        tinytcp::ConnectedSocket connect(host, port);
        static const int bufferMaxLen = 4096;
        std::array<char, bufferMaxLen> buffer = { 0 };

        connect.putData(sendStr.c_str(), sendStr.size());
        
        int rcvBytes = connect.getData(&buffer[0], bufferMaxLen);
        
        blockingPrint(str::createMultiStr("Recv ", rcvBytes, " bytes on fd ", connect.getSocket()));
    }
    catch(const std::exception& ex) {
        blockingPrint(str::createMultiStr("Exception thread #", threadId, ": ", ex.what()));
    }
    catch(...) {
        blockingPrint("Unknown exception...");
    }
}

void blockingPrint(const std::string& str) {
    std::lock_guard<std::mutex> lck(printMtx);
    cout << str << endl;
}
