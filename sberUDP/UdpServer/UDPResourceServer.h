#pragma once

#include <unordered_map>
#include <string>
#include <winsock2.h>

const int MAX_RESOURCE_NAME_LENGTH = 64;
const int MAX_RESOURCE_CONTENT_LENGTH = 1024;

class UDPResourceServer {
public:
    UDPResourceServer(const char* resourcesFilename);
    ~UDPResourceServer();

    void Start();

private:
    void LoadResources(const char* filename);
    void HandleRequest(const char* buffer, int length, SOCKADDR_IN* clientAddr);

private:
    std::unordered_map<std::string, std::string> _resources;
    SOCKET _socket;
    SOCKADDR_IN _serverAddr;
};
