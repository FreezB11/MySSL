#include <iostream>
#include <string>
#include <fstream>
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib,"ws2_32.lib")
#elif __linux__
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
#else
    #error "Unknown operating system"
#endif
#include "color.h"
#define log(x) std::cout << x << std::endl;
#define clog(x,y) std::cout << y << x << COLOR_RESET << std::endl;
#define tlog(x,y) std::cout << YEL << "[" << currentDateTime() << "] " << y << x << COLOR_RESET << std::endl;
#include <time.h>
#define main main()


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
int main{
    WSADATA wsa;
	SOCKET s;
    struct sockaddr_in server;
    #ifdef _WIN32

        // std::cout << "Intializing......." << std::endl;
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
	    	std::cout << "Failed. Error Code : %d" << WSAGetLastError()  << std::endl;
	    }

    #endif
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		std::cout << "Could not create socket : %d" << WSAGetLastError() << std::endl;
	}
    tlog("Socket Created",CYN)

    server.sin_addr.s_addr = inet_addr("20.207.73.88");
    server.sin_family = AF_INET;
	server.sin_port = htons(80);

    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
	}
    tlog("connected",BLU)

    std::string request;
    std::string  header = "GET / HTTP/1.1\r\n";
    std::string host = "Host: github.com\r\n";
    request += header;
    request += host;

    if(send(s,request.c_str(),request.size(),0) == -1){
         std::cout << "err" << std::endl;
    }
    tlog("Request sent",BGRN)

    int recv_len;
    char buffer[2048];

    recv_len = recv(s,buffer,sizeof(buffer),0);
    log(recv_len);
    for(int i = 0;i < recv_len;i++){
        std::cout << buffer[i];
    }
    std::cout << std::endl;
    tlog("Response recieved",GRN)
    return 0;
}