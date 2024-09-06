#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

void InitializeWinsock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        exit(EXIT_FAILURE);
    }
}

SOCKET CreateSocket(const char* hostname, const char* port) {
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int res = getaddrinfo(hostname, port, &hints, &result);
    if (res != 0) {
        std::cerr << "getaddrinfo failed: " << res << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    SOCKET ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    res = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if (res == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!" << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return ConnectSocket;
}

SSL_CTX* InitializeSSL() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (!ctx) {
        std::cerr << "Unable to create SSL context" << std::endl;
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

void Cleanup(SOCKET socket, SSL_CTX* ctx, SSL* ssl) {
    SSL_free(ssl);
    closesocket(socket);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    WSACleanup();
}

int main() {
    const char* hostname = "www.example.com";
    const char* port = "443";

    InitializeWinsock();
    SOCKET ConnectSocket = CreateSocket(hostname, port);
    SSL_CTX* ctx = InitializeSSL();
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, ConnectSocket);

    if (SSL_connect(ssl) <= 0) {
        std::cerr << "SSL connect failed" << std::endl;
        ERR_print_errors_fp(stderr);
        Cleanup(ConnectSocket, ctx, ssl);
        return EXIT_FAILURE;
    }

    const char* request = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\n\r\n";
    SSL_write(ssl, request, strlen(request));

    char buffer[4096];
    int bytes_read;
    while ((bytes_read = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        std::cout << buffer;
    }

    Cleanup(ConnectSocket, ctx, ssl);
    return 0;
}
