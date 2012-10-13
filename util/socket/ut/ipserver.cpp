#include "../socket.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You must set port as program parameter" << std::endl;
        return 1;
    }

    TMbTCPSocket serv(atoi(argv[1]));
    
    serv.Bind();
    serv.Listen();
    
    while(1) {
    TMbTCPSocket client = serv.Accept();
    
    std::cout << "Get connection from " << client.getIp() << std::endl;
    std::cout << "Sending him his IP address!" << std::endl;
    
    client.Send(client.getIp());

    std::cout << "OK. Closing connection..." << std::endl;
    client.Close();
    }

    return 0;
}
