#include "UDPResourceServer.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        UDPResourceServer server("resources.txt");
        server.Start();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
