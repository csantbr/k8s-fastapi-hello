#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

std::atomic<bool> running(true);

void signal_handler(int signal) {
    std::cout << "Received signal " << signal << ", shutting down..." << std::endl;
    running = false;
}

void handle_client(SOCKET client_socket) {
    const char* response = "Hello World from C++!\n";
    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
}

int main() {
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGINT, signal_handler);

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }
#endif

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_socket);
        return 1;
    }

    if (listen(server_socket, 10) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(server_socket);
        return 1;
    }

    std::cout << "C++ TCP Server running on port 8000..." << std::endl;
    std::cout << "Healthcheck: TCP socket probe on port 8000" << std::endl;

    // Set socket to non-blocking for graceful shutdown
    #ifndef _WIN32
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    #endif

    while (running) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket != INVALID_SOCKET) {
            std::thread(handle_client, client_socket).detach();
        }
    }

    closesocket(server_socket);
    
#ifdef _WIN32
    WSACleanup();
#endif

    std::cout << "Server stopped." << std::endl;
    return 0;
}
