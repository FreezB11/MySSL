#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Function to initialize SSL library
SSL_CTX* init_ssl_context() {
    SSL_CTX *ctx;
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}

// Function to create a socket and connect to a server
int create_socket(const char* hostname, int port) {
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &serv_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int main() {
    const char* hostname = "103.102.166.224"; // Google IP address, you can use "google.com" with a DNS resolver instead.
    const int port = 443;
    char *path;
    // if(argc>=2){
    //     path = argv[1];
    // }
    // Initialize SSL
    SSL_CTX *ctx = init_ssl_context();
    SSL *ssl = SSL_new(ctx);

    // Create socket and connect
    int sockfd = create_socket(hostname, port);

    // Attach SSL to the socket
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) != 1) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send HTTPS request
    const char* request = "GET /wiki/India HTTP/1.1\r\nHost: en.wikipedia.org\r\nConnection: close\r\n\r\n";
    if (SSL_write(ssl, request, strlen(request)) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read response
    char buffer[4096];
    int bytes;
    while ((bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        std::cout << buffer;
    }
    std::cout << "ntg" << std::endl;
    // Cleanup
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);

    return 0;
}
