#include "../TSocket.h"

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
        
        client.Write("Hello, Neo!");
        ans = client.Read();
        std::cout << "Client: " << ans << std::endl;

        client.Write("Follow the white rabbit!");
        ans = client.Read();
        std::cout << "Client: " << ans << std::endl;

        client.Write("Knock knock, Neo!");

        //std::cout << "OK" << std::endl;
    }
    else if ( pid > 0 ) {
        std::string sans = "";
        TSocket cli("127.0.0.1", atoi(argv[1]));
        
        cli.Connect();
        sans = cli.Read();
        std::cout << "Server: " << sans << std::endl;
        
        cli.Write("Who are you?");
        sans = cli.Read();
        std::cout << "Server: " << sans << std::endl;
    
        cli.Write("What a white rabbit?");
        sans = cli.Read();
        std::cout << "Server :" << sans << std::endl;
    }

    return 0;
}
