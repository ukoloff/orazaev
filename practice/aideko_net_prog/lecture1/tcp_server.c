/**
    exersize "UDP Server" from unix network programming lectures.

    link: http://habrahabr.ru/company/ideco/blog/138322/

    gmail me: AOrazaev[at]gmail.com
*/
#include<sys/socket.h>
#include<arpa/inet.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"definitions.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: you must send port numver as prog arg.\n");
        exit(1);
    }
    uint16_t port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Can't bind socket on port %d\n", port);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        fprintf(stderr, "Can't set socket %d to listen socket\n", port);
        exit(2);
    }

    struct sockaddr_in cli_addr;
    size_t sockaddr_in_size = sizeof(cli_addr);
    int clifd = accept(sockfd, (struct sockaddr *) &cli_addr, &sockaddr_in_size);
   
    char buffer[BUF_SIZE];
    while (1) {
        if (recv(clifd, buffer, BUF_SIZE, 0) != -1) {
            printf("%s\n", buffer);
        }
    }
    
    close(sockfd);
    return 0;
}

