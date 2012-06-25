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

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        fprintf(stderr, "Can't bind socket on port %d", port);
        exit(1);
    }

    char buffer[BUF_SIZE];
    while (1) {
        if (recv(sockfd, buffer, BUF_SIZE, 0) != -1) {
            printf("%s\n", buffer);
        }
    }
    
    close(sockfd);
    return 0;
}

