#include <iostream>
#include <string>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

void initSockets() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        exit(EXIT_FAILURE);
    }
#endif
}

void cleanupSockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

int main() {
    const std::string host = "en.wikipedia.org";
    const std::string path = "/wiki/Korea";
    const std::string port = "80"; // HTTP port

    initSockets();

    // Create socket
    int sock;
#ifdef _WIN32
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        cleanupSockets();
        return EXIT_FAILURE;
    }
#else
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return EXIT_FAILURE;
    }
#endif

    // Get host address
    struct hostent *server = gethostbyname(host.c_str());
    if (server == nullptr) {
        std::cerr << "Host not found." << std::endl;
        cleanupSockets();
        return EXIT_FAILURE;
    }

    // Set up the sockaddr_in structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        cleanupSockets();
        return EXIT_FAILURE;
    }

    // Send HTTP GET request
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";

    if (send(sock, request.c_str(), request.length(), 0) < 0) {
        std::cerr << "Send failed." << std::endl;
        cleanupSockets();
        return EXIT_FAILURE;
    }

    // Receive response
    char buffer[4096];
    std::string response;
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response += buffer;
    }

    if (bytes_received < 0) {
        std::cerr << "Receive failed." << std::endl;
    } else {
        std::cout << response << std::endl;
    }

    // Close socket
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    cleanupSockets();
    return 0;
}
