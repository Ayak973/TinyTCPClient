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

#include<sys/socket.h>	//socket
#include<arpa/inet.h>	//inet_addr
#include<netdb.h>	//hostent

using std::cout;
using std::endl;

static const unsigned int parallelJobsCount = 4;
static const int port = 8080;
static const std::string host = "127.0.0.1";
static const std::string sendStr = "Hello";


void sendAndReceive(int threadId);

int main(int argc, char** argv) {
    cout << "Starting client..." << endl;
    
    //socket file descriptor array
    std::array<int, parallelJobsCount> fdArray = { 0 };
    std::array<std::thread, parallelJobsCount> threadArray;
    
    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        
        threadArray[i] = std::thread(sendAndReceive, i);
        
    }

    for (unsigned int i = 0; i < parallelJobsCount; ++i) {
        
        if (threadArray[i].joinable()) threadArray[i].join();
        
    }
    
    cout << "Client finished !" << endl;
    
    return 0;
}

void sendAndReceive(int threadId) {
    cout << "Starting thread #" << threadId << "..." << endl;
    
    
    
    cout << "Thread #" << threadId << " stopped !" << endl;
}


