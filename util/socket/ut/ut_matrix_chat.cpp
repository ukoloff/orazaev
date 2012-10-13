#include "../socket.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "You must set port as program parameter" << std::endl;
        return 1;
    }

    TSocket serv(atoi(argv[1]));
    
    serv.Bind();
    serv.Listen();
    
    int pid = fork();
    if ( pid < 0) {
        std::cerr << "Can't fork new process" << std::endl;
        return 1;
    }
    if ( pid == 0 ) {
        TSocket client = serv.Accept();
        std::string ans = "";
    
        std::cout << "Server: Get connection from " << client.getIp() << std::endl;
        
        client.Send("Hello, Neo!");
        ans = client.Recv();
        std::cout << "Client: " << ans << std::endl;

        client.Send("Follow the white rabbit!");
        ans = client.Recv();
        std::cout << "Client: " << ans << std::endl;

        client.Send("Knock knock, Neo!");

        //std::cout << "OK" << std::endl;
    }
    else if ( pid > 0 ) {
        std::string sans = "";
        TSocket cli("127.0.0.1", atoi(argv[1]));
        
        cli.Connect();
        sans = cli.Recv();
        std::cout << "Server: " << sans << std::endl;
        
        cli.Send("Who are you?");
        sans = cli.Recv();
        std::cout << "Server: " << sans << std::endl;
    
        cli.Send("What a white rabbit?");
        sans = cli.Recv();
        std::cout << "Server :" << sans << std::endl;
    }

    return 0;
}
