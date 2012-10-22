/**
    exersize "TCP Sender" from unix network programming lectures.

    link: http://habrahabr.ru/company/ideco/blog/138322/

    gmail me: AOrazaev[at]gmail.com
*/
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#include<unistd.h>
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
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
        fprintf(stderr, "Can't connect to socket on port %d", port);
        exit(1);
    }

    char buffer[BUF_SIZE];
    int second = 0;
    while (++second < MAX_SECOND) {
        sprintf(buffer, "%s %d", STR_MSG, second);
        if (send(sockfd, buffer, BUF_SIZE, MSG_NOSIGNAL) == BUF_SIZE) {
            printf("%s\n", buffer);
        }
        sleep(1);
    }
    
    close(sockfd);
    return 0;
}

