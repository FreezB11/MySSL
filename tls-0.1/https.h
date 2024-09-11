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

#define Port 80
#define Ports 443
#include "tls.h"

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

#define MAX_GET_COMMAND 255
#define BUFFER_SIZE 255

const std::string currentDateTime();

class https{
private:
    TLSParameters tls_context;
    struct hostent *host_name;
    struct sockaddr_in host_address;
#ifdef _WIN32
    WSADATA wsa;
    SOCKET sock;
#elif __linux__ 
    int sock;
#endif
    char *host;
    char *path;
    int master_secret_length;
    unsigned char *master_secret;
    int session_id_length;
    unsigned char *session_id;
public:
    https(char *URI);
    int httpsConnect(const char *HOST,const char *PATH);
    int httpsSend(int sock, const char *path, const char *host, TLSParameters *tls_context);
    void httpsRecv(int sock, TLSParameters *tls_connect);
};


#endif