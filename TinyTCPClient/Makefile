all: tinyTCPClient

tinyTCPClient: main.o
        g++ -Wall -std=c++14 -o tinyTCPClient main.o -lpthread

tinytcpclient.o:main.cpp
        g++ -Wall -std=c++14 -c main.cpp -lpthread

clean:
        rm main.o tinyTCPClient
