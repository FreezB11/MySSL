#ifndef _HTTPS_
#define _HTTPS_

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib,"ws2_32.lib")
#elif __linux__
    #include <arpa/inet.h>
    #include <sys/socket.h>    
    #include <unistd.h>
    #include <netdb.h>
    #include <netinet/in.h>
#else
    #error "Unknown operating system"
#endif

#define port 80
#define ports 443

/*
    Utils
*/
#include <time.h>
#include <string>
#include <iostream>

#include "color.h"
#define log(x) std::cout << x << std::endl;
#define clog(x,y) std::cout << y << x << COLOR_RESET << std::endl;
#define tlog(x,y) std::cout << YEL << "[" << currentDateTime() << "] " << y << x << COLOR_RESET << std::endl;


const std::string currentDateTime();

class https{
private:
#ifdef _WIN32
    WSADATA wsa;
    SOCKET sock;
#elif __linux__ 
    int sock;
#endif
    char *host;
    char *path;
public:
    https(char *URI);
};


#endif