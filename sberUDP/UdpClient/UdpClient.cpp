#include "UdpClient.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

UDPResourceClient::UDPResourceClient() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка инициализации Winsock" << std::endl;
        throw std::runtime_error("Ошибка инициализации Winsock");
    }

    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (_socket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        WSACleanup();
        throw std::runtime_error("Ошибка создания сокета");
    }

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &_serverAddr.sin_addr);
}

UDPResourceClient::~UDPResourceClient() {
    closesocket(_socket);
    WSACleanup();
}

void UDPResourceClient::SendRequest(const char* resourceName) {
    int bytesSent = sendto(_socket, resourceName, strlen(resourceName), 0, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Ошибка отправки запроса" << std::endl;
        throw std::runtime_error("Ошибка отправки запроса");
    }

    char buffer[MAX_BUFFER_SIZE] = { 0 };
    int bytesReceived = recvfrom(_socket, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
    if (bytesReceived > 0) {
        std::cout << buffer << std::endl;
    }
    else {
        std::cerr << "Ошибка получения ответа" << std::endl;
        throw std::runtime_error("Ошибка получения ответа");
    }
}
