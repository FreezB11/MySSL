#ifndef _HTTP_
#define _HTTP_

#include <iostream>
#include <string>
#include <fstream>
#include "openssl/ssl.h"
#include "openssl/err.h"

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

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

class http {
private:
    WSADATA wsa;
    SOCKET s;
    int received;
    struct sockaddr_in server;
    SSL_CTX* ctx;
    SSL* ssl;

public:
    http(const std::string& msg, const char* IP);
    int httpsReq(const std::string& host, const std::string& path);
    void httpsRes(const std::string& filename);
    ~http();
};

http::http(const std::string& msg, const char* IP) {
    log(msg)
#ifdef _WIN32
    tlog("Initializing", GRN)
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << HRED << "Failed. Error Code: " << WSAGetLastError() << COLOR_RESET << std::endl;
        return;
    }
    tlog("Initialized", GRN)
#endif
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << HRED << "Could not create socket: " << WSAGetLastError() << COLOR_RESET << std::endl;
    }

    tlog("Socket Created", CYN)
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(443);  // HTTPS port

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "connect error" << std::endl;
        return;
    }

    tlog("Connected", BLU)

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        std::cerr << "Failed to create SSL context" << std::endl;
        return;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, s);
    if (SSL_connect(ssl) <= 0) {
        std::cerr << "SSL connection failed" << std::endl;
        ERR_print_errors_fp(stderr);
    }
    tlog("SSL connection established", GRN)
}

inline int http::httpsReq(const std::string& host, const std::string& path) {
    std::string request = "GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";

    if (SSL_write(ssl, request.c_str(), request.size()) <= 0) {
        tlog("Failed to send request!", BRED)
        ERR_print_errors_fp(stderr);
        return -1;
    }
    tlog("Request sent", BGRN)
    return 0;
}

void http::httpsRes(const std::string& filename) {
    char buffer[4096];
    int bytes;

    std::ofstream outfile(filename, std::ios::binary);

    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer))) > 0) {
        outfile.write(buffer, bytes);
    }

    if (bytes < 0) {
        std::cerr << "SSL read error" << std::endl;
        ERR_print_errors_fp(stderr);
    }

    outfile.close();
    tlog("Response received", GRN)
}

http::~http() {
    SSL_shutdown(ssl);
    SSL_free(ssl);
    closesocket(s);
    SSL_CTX_free(ctx);
#ifdef _WIN32
    WSACleanup();
#endif
}

#endif
