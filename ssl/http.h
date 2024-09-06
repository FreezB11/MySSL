#ifndef _HTTP_
#define _HTTP_

#define HTTP_PORT 80
#define HTTPS_PORT 443

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
    #include <netdb.h>
    #include <netinet/in.h>
#else
    #error "Unknown operating system"
#endif
#include "color.h"
#define log(x) std::cout << x << std::endl;
#define clog(x,y) std::cout << y << x << COLOR_RESET << std::endl;
#define tlog(x,y) std::cout << YEL << "[" << currentDateTime() << "] " << y << x << COLOR_RESET << std::endl;

#include <string>
#include <stdio.h>
#include <time.h>
#include "tls/tls.h"
#include "tls/hex.h"
// #include <unistd.h>

/*socks defines
// IPv4 AF_INET sockets:
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};


typedef struct in_addr {
  union {
    struct {
      u_char s_b1,s_b2,s_b3,s_b4;
    } S_un_b;
    struct {
      u_short s_w1,s_w2;
    } S_un_w;
    u_long S_addr;
  } S_un;
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;


struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
*/

#define ip "127.0.0.1"

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

class http{
private:
    WSADATA wsa;
    TLSParameters tls_context;
	SOCKET s;
    int received;
    struct sockaddr_in server;
    int master_secret_length;
    unsigned char *master_secret;
    int session_id_length;
    unsigned char *session_id;
public:
    http(std::string msg,const char* IP);
    int httpReq(const std::string host,const std::string path);
    void TLSinit();
    void httpRes(const std::string& filename);
    // ~http();
};

http::http(std::string msg,const char* IP){
    log(msg)
    #ifdef _WIN32
        tlog("Intializing",GRN)
        // std::cout << "Intializing......." << std::endl;
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
	    	std::cout << HRED << "Failed. Error Code : %d" << WSAGetLastError() << COLOR_RESET << std::endl;
	    }
	    tlog("Initialized",GRN)
    #endif
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		std::cout << HRED << "Could not create socket : %d" << WSAGetLastError() << COLOR_RESET << std::endl;
	}

	tlog("Socket Created",CYN)
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
	server.sin_port = htons(HTTPS_PORT);

	//Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
	}
	
	tlog("connected",BLU)
}

inline int http::httpReq(const std::string Host, const std::string path){
    std::string request;
    std::string  header = "GET" + path + "HTTP/1.1\r\n";
    std::string host = "Host: " + Host + "\r\n";
    std::string accpl = "Accept-Language: en-US,en;q=0.5";
    std::string accp = "Accept-encoding: gzip, deflate, br, zstd";
    std::string usr = "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0";
    std::string ref = "Referer: https://github.com/FreezB11/Ikigai";
    request += header;
    request += host;
    // request += usr;
    // request += accpl;
    // request += accp;
    // request += ref;
    // request += "Connection: keep-alive \r\n";
    // request += "Upgrade-Insecure-Requests: 1 \r\n If-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT \r\n If-None-Match: 'c561c68d0ba92bbeb8b0fff2a9199f722e3a621a' \r\nCache-Control: max-age=0";

    if(tls_send(s,request.c_str(),request.size(),0,&tls_context) == -1){
        tlog("Failed to send request!!",BRED)
        return -1;
    }
    tlog("Request sent",BGRN)

    return 0;
}

void http::TLSinit(){
    tlog("Connection complete; negotiating TLS parameters\n",BLU)
    if ( session_id != NULL )
  {
    if ( tls_resume( s, session_id_length,
         session_id, master_secret, &tls_context ) )
    {
      fprintf( stderr, "Error: unable to negotiate SSL connection.\n" );
      // if ( close( s ) == -1 )
      // {
      //   perror( "Error closing client connection" );
      // }
    }
  }
  else
  {
    if ( tls_connect( s, &tls_context, 0 ) )
    {
      fprintf( stderr, "Error: unable to negotiate TLS connection.\n" );
    }
  }
}

void http::httpRes(const std::string& filename){
    int recv_len;
    char buffer[2048];

    recv_len = tls_recv(s,buffer,sizeof(buffer),0,&tls_context);
    log(recv_len)
    for(int i = 0;i < recv_len;i++){
        std::cout << buffer[i];
    }
    std::cout << std::endl;
    tlog("Response recieved",GRN)
    tls_shutdown(s, &tls_context);
}

// void http::httpRes(const std::string& filename) {
//     const int buffer_size = 16384;
//     char buffer[buffer_size];
//     std::ofstream outfile(filename, std::ios::binary);
//     bool header_done = false;
//     std::string header;
//     int content_length = -1;

//     while (true) {
//         received = recv(s, buffer, buffer_size, 0);
//         puts(buffer);
//         break;
//         if (received <= 0) break;

//         if (!header_done) {
//             header.append(buffer, received);

//             size_t pos = header.find("\r\n\r\n");
//             if (pos != std::string::npos) {
//                 header_done = true;
//                 std::string body = header.substr(pos + 4);
//                 outfile.write(body.c_str(), body.size());

//                 if (header.find("Content-Length:") != std::string::npos) {
//                     content_length = std::stoi(header.substr(header.find("Content-Length:") + 16));
//                 }
//             }
//         } else {
//             outfile.write(buffer, received);
//         }
//     }

//     outfile.close();
//     tlog("Response recieved",GRN)
// }



#endif
