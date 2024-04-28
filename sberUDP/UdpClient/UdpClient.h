#pragma once

#include <string>
#include <WinSock2.h>

const int MAX_BUFFER_SIZE = 1024;

class UDPResourceClient {
public:
    UDPResourceClient();
    ~UDPResourceClient();

    void SendRequest(const char* resourceName);

private:
    SOCKET _socket;
    SOCKADDR_IN _serverAddr;
};
