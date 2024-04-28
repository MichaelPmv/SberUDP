#include "UDPResourceServer.h"
#include <iostream>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

UDPResourceServer::UDPResourceServer(const char* resourcesFilename) {
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
    _serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_socket, (SOCKADDR*)&_serverAddr, sizeof(_serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        closesocket(_socket);
        WSACleanup();
        throw std::runtime_error("Ошибка привязки сокета");
    }

    LoadResources(resourcesFilename);
}

UDPResourceServer::~UDPResourceServer() {
    closesocket(_socket);
    WSACleanup();
}

void UDPResourceServer::Start() {
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[MAX_RESOURCE_NAME_LENGTH + 1] = { 0 };

    while (true) {
        int bytesReceived = recvfrom(_socket, buffer, MAX_RESOURCE_NAME_LENGTH, 0, (SOCKADDR*)&clientAddr, &clientAddrLen);
        if (bytesReceived > 0) {
            HandleRequest(buffer, bytesReceived, &clientAddr);
        }
    }
}

void UDPResourceServer::LoadResources(const char* filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos) {
                std::string name = line.substr(0, spacePos);
                std::string content = line.substr(spacePos + 1);
                _resources[name] = content;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Невозможно открыть файл ресурсов" << std::endl;
        throw std::runtime_error("Невозможно открыть файл ресурсов");
    }
}

void UDPResourceServer::HandleRequest(const char* buffer, int length, SOCKADDR_IN* clientAddr) {
    std::string resourceName(buffer, length);
    std::cout << _resources[resourceName] << std::endl;
    if (_resources.count(resourceName)) {
        std::string response = "-BEGIN-\n" + _resources[resourceName] + "\n-END-";
        sendto(_socket, response.c_str(), response.length(), 0, (SOCKADDR*)clientAddr, sizeof(*clientAddr));
    }
    else {
        std::string errorResponse = "-ERROR-\nResource not found\n-END-";
        sendto(_socket, errorResponse.c_str(), errorResponse.length(), 0, (SOCKADDR*)clientAddr, sizeof(*clientAddr));
    }
}
