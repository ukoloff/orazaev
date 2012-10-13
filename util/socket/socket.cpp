#include "socket.h"
#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include <cerrno>
#include <memory>

static int errcode;

TTCPSocket::TTCPSocket(const std::string& hostname, const int& portno) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        throw ESocket("Error: can't create socket!");
    }

    memset(&addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(portno);

    struct hostent* serv_name = gethostbyname(hostname.c_str());
    bcopy((char *) serv_name->h_addr, (char *) &(this->addr.sin_addr.s_addr), serv_name->h_length);
}

TTCPSocket::TTCPSocket(const int& Sockfd, const struct sockaddr_in& Addr) 
    : sockfd(Sockfd)
    , addr(Addr) {
}

TTCPSocket::TTCPSocket(const int& portno) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        throw ESocket("Error: can't create socket!");
    }

    memset(&addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(portno);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

TTCPSocket::~TTCPSocket() {
}

void TTCPSocket::Connect() {
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw ESocket("Error: can't connect to host!");
    }   
}

void TTCPSocket::Listen() {
    if (listen(sockfd, 5) < 0) {
        throw ESocket("Error: can't set port as listening!");
    }
}

void TTCPSocket::Bind() {
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw ESocket("Error: can't bind socket!");
    }
}

TTCPSocket TTCPSocket::Accept() {
    struct sockaddr_in cli_addr;
    int conn;
    unsigned int tsize = sizeof(cli_addr);

    memset(&cli_addr, 0, tsize);
    conn = accept(sockfd, (struct sockaddr *) &cli_addr, &tsize);

    if (conn < 0) {
        throw ESocket("Error: can't accepting connection!");
    }
    
    return TTCPSocket(conn, cli_addr);
}

void TTCPSocket::Close() {
        close(sockfd);
}


void TTCPSocket::Send(const std::string & msg, size_t size) {
    if (size > CBUF_SIZE) {
        throw ESocket("Size of message is bigger than buffer");
    }
    char buf[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * sizeof(char));

    memcpy(buf, msg.c_str(), size);
    if (send(sockfd, buf, CBUF_SIZE * sizeof(char), MSG_NOSIGNAL) < 0) {
        throw ESocket("Error: can't send message to socket!");
    }
}

void TTCPSocket::Send(const std::string & msg) {
    return Send(msg, msg.size());
}

std::string TTCPSocket::Recv(size_t size) {
    if (size > CBUF_SIZE) {
        throw ESocket("Size of recved message is bigger than buffer!");
    }
    char buf[CBUF_SIZE + 1];
    memset(buf, 0, CBUF_SIZE * sizeof(char));

    size_t totalReceved = 0;
    while(totalReceved != size) {
        size_t receved = recv(sockfd, buf + totalReceved, size, 0);
        
        if (receved < 0) {
            throw ESocket("Error: can't recv message from socket!");
        }

        totalReceved += receved;
    }
    buf[totalReceved] = '\0';
    return std::string(buf);
}

std::string TTCPSocket::Recv() {
    return Recv(CBUF_SIZE);
}


std::string TTCPSocket::Read() {
    return Recv();
}

void TTCPSocket::Write(const std::string& msg) {
    Send(msg);
}

void TTCPSocket::Write(const std::string& msg, size_t size) {
    Send(msg, size);
}

std::string TTCPSocket::getIp() {
    char ipAddr[CBUF_SIZE];
    memset(ipAddr, 0, CBUF_SIZE * sizeof(char));
    
    inet_ntop(AF_INET, (in_addr *) &(addr.sin_addr), ipAddr, CBUF_SIZE);
    
    return std::string(ipAddr);
}
