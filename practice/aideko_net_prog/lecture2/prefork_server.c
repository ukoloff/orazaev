#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#include<stdio.h>
#include<stdlib.h>


int main(int argc, char** argv) {
    /** Operate SIGCHLD */
    struct sigaction saction;
    saction.sa_handler = SIG_DFL;
    sigemptyset(&saction.sa_mask);
    saction.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    sigaction(SIGCHLD, &saction, 0);

    if (argc < 2) {
        fprintf(stderr, "You should set prort number as prog arg.\n");
        exit(1);
    }
    uint16_t portno = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(portno);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Error: can't bind socket on port %d\n", portno);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        fprintf(stderr, "Error: can't set socket %d as listen socket\n", portno);
        exit(1);
    }

    int conn;
    struct sockaddr_in cli_addr;
    size_t cli_addr_size = sizeof(cli_addr);
    while(1) {
        conn = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_addr_size);
        if (conn < 0) 
            continue;
        
        int child = fork();
        while (child < 0) 
            child = fork();
        if (child == 0) {

            int x = 0;
            for(; x < 5; x++) {
                send(conn, "Hi, there!\n", 12, 0);
                sleep(1);
            }
            
            printf("closing sockets by PID = %d, PPID = %d\n", getpid(), getppid());
            close(sockfd);
            close(conn);
            return 0;
        }
        while(close(conn) < 0);
    }
    close(sockfd);
    
    return 0;
}
