#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void runServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Server: Socket creation failed!" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Server: Bind failed!" << std::endl;
        closesocket(serverSocket);
        return;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Server: Listen failed!" << std::endl;
        closesocket(serverSocket);
        return;
    }

    std::cout << "Server: Listening on port " << SERVER_PORT << "..." << std::endl;

    SOCKET clientSocket;
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Server: Accept failed!" << std::endl;
    }
    else {
        char buffer[BUFFER_SIZE];
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Server: Received message: " << buffer << std::endl;

            std::string response = "Hello from Server!";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
}

void runClient() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Client: Socket creation failed!" << std::endl;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Client: Connection failed!" << std::endl;
        closesocket(clientSocket);
        return;
    }

    std::string message = "Hello from Client!";
    send(clientSocket, message.c_str(), message.size(), 0);

    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Client: Received message: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
}

int main() {
    // Запуск сервера и клиента в отдельных потоках
    std::thread serverThread(runServer);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Подождать, пока сервер запустится
    std::thread clientThread(runClient);

    serverThread.join();
    clientThread.join();

    std::cout << "Test cycle completed!" << std::endl;
    return 0;
}
