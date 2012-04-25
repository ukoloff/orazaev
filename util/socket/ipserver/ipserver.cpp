#include "../TSocket.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You must set port as program parameter" << std::endl;
        return 1;
    }

    TSocket serv(atoi(argv[1]));
    
    serv.Bind();
    serv.Listen();
    
    while(1) {
    TSocket client = serv.Accept();
    
    std::cout << "Get connection from " << client.getIp() << std::endl;
    std::cout << "Sending him his IP address!" << std::endl;
    
    client.Write(client.getIp());

    std::cout << "OK" << std::endl;
    }

    return 0;
}
