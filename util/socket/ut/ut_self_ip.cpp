#include "../socket.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You must set port as program parameter" << std::endl;
        return 1;
    }

    TMbTCPSocket serv(atoi(argv[1]));
    
    serv.Bind();
    serv.Listen();
    
    int pid = fork();
    if ( pid < 0) {
        std::cerr << "Can't fork new process" << std::endl;
        return 1;
    }
    if ( pid == 0 ) {
        TMbTCPSocket client = serv.Accept();
    
        std::cout << "Get connection from " << client.getIp() << std::endl;
        std::cout << "Sending him his IP address!" << std::endl;
    
        client.Send(client.getIp());

        std::cout << "OK" << std::endl;
    }
    else if ( pid > 0 ) {
        std::cout << "Client: I was forked! pid = " << pid << std::endl;
        TMbTCPSocket cli("127.0.0.1", atoi(argv[1]));
        
        cli.Connect();
        
        std::string ans = cli.Recv();
        
        std::cout << "Client: Got answer = " << ans << std::endl;
    }

    return 0;
}
