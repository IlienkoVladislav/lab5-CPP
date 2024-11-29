#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 54001
#define BUFFER_SIZE 1024

void runUDPServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket!" << std::endl;
        closesocket(serverSocket);
        return;
    }

    std::cout << "UDP Server listening on port " << PORT << std::endl;

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        std::string response = "Hello from UDP Server!";
        sendto(serverSocket, response.c_str(), response.size(), 0, (sockaddr*)&clientAddr, clientAddrSize);
    }

    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    runUDPServer();
    return 0;
}
