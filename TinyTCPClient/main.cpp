/* 
 * File:   main.cpp
 * Author: Ayak973
 *
 * Created on December 5, 2017, 19:01
 */

#include <iostream>
#include <string>
#include <array>
#include <thread>
#include <mutex>

#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<netdb.h>	//hostent

using std::cout;
using std::endl;

static const unsigned int parallelJobsCount = 4;
static const int port = 8080;
static const std::string host = "127.0.0.1";
static const std::string sendStr = "Hello";

//std::cout synchronization
std::mutex printMtx;

void sendAndReceive(unsigned int threadId);
bool createSocketAndConnect(std::string& error);
void blockingPrint(const std::string& str);

int main(int argc, char** argv) {
    cout << "Starting client..." << endl;
    
    //socket file descriptor array
    std::array<int, parallelJobsCount> fdArray = { 0 };
    std::array<std::thread, parallelJobsCount> threadArray;
    
    //
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        threadArray[i] = std::thread(sendAndReceive, i);
    }

    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        if (threadArray[i].joinable()) threadArray[i].join();
    }
    
    cout << "Client finished !" << endl;
    
    return 0;
}

void sendAndReceive(unsigned int threadId) {
    blockingPrint("Starting thread #" + std::to_string(threadId) + "...");
    
    std::string error;
    int clientfd = -1;
    
    clientfd = createSocketAndConnect(error);
    if (clientfd < 0) {
        blockingPrint("Thread #" + std::to_string(threadId) + " error: " + error + " !");
        return;
    }
    
    
    
    
    close(fd);
    
    blockingPrint("Thread #" + std::to_string(threadId) + " stopped !");
}

bool createSocketAndConnect(std::string& error) {
    int fd = socket(AF_INET , SOCK_STREAM , 0);
    
    if (fd < 0) {
        error = "cannot create socket";
        return false;
    }
    
    
    
    
    return true;
}

void blockingPrint(const std::string& str) {
    std::lock_guard<std::mutex> lck(printMtx);
    cout << str << endl;
}

