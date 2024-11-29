#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54001
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void runUDPClient() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket!" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    std::string message = "Hello from UDP Client!";
    sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

    char buffer[BUFFER_SIZE];
    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    runUDPClient();
    return 0;
}
