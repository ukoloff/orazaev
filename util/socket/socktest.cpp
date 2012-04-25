#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstring>
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return 1;
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) return 2;
    
    if (listen(sockfd, 5) < 0) return 3;

    std::cout << "Ready to listen" << std::endl;

    memset(&cli_addr, 0, sizeof(cli_addr));
    unsigned int cli_size = sizeof(cli_addr);
    int connfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_size);
   
    std::cout << "Get connection!" << std::endl;

    char cli_ip[50];
    memset(&cli_ip, 0, sizeof(cli_ip));
    inet_ntop(AF_INET, (in_addr *) &(cli_addr.sin_addr), cli_ip, 50);

    std::cout << cli_ip << std::endl;

    return 0;
}
