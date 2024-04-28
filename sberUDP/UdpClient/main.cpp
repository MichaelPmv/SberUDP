#include "UdpClient.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: client.exe <имя ресурса>" << std::endl;
        return 1;
    }
    try {
        UDPResourceClient client;
        client.SendRequest(argv[1]);
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
