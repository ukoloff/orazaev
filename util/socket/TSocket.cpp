#include "TSocket.h"
#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include <cerrno>
#include <memory>

static int errcode;

TSocket::TSocket(const std::string& hostname, const int& portno) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        std::cerr << "Error: can't create socket!" << std::endl;
        throw ESocket();
    }

    memset(&addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(portno);

    struct hostent* serv_name = gethostbyname(hostname.c_str());
    bcopy((char *) serv_name->h_addr, (char *) &(this->addr.sin_addr.s_addr), serv_name->h_length);
}

TSocket::TSocket(const int& Sockfd, const struct sockaddr_in& Addr) 
    : sockfd(Sockfd)
    , addr(Addr) {
}

TSocket::TSocket(const int& portno) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        std::cerr << "Error: can't create socket!" << std::endl;
        throw ESocket();
    }

    memset(&addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(portno);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}

TSocket::~TSocket() {
}

void TSocket::Connect() {
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        //std::cerr << "Error: can't connect to host!" << std::endl;
        throw EConnect();
    }   
}

void TSocket::Listen() {
    if (listen(sockfd, 5) < 0) {
        std::cerr << "Error: can't set port as listening!" << std::endl;
        throw EListen();
    }
}

void TSocket::Bind() {
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        std::cerr << "Error: can't bind socket!" << std::endl;
        throw EBind();
    }
}

TSocket TSocket::Accept() {
    struct sockaddr_in cli_addr;
    int conn;
    unsigned int tsize = sizeof(cli_addr);

    memset(&cli_addr, 0, tsize);
    conn = accept(sockfd, (struct sockaddr *) &cli_addr, &tsize);

    if (conn < 0) {
        std::cout << "Error: can't accepting connection!" << std::endl;
        throw EAccept();
    }
    
    return TSocket(conn, cli_addr);
}

void TSocket::Close() {
        close(sockfd);
}

void TSocket::Write(std::string msg) {
    char buf[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * sizeof(char));
    
    memcpy(buf, msg.c_str(), msg.size());
        
    if (write(sockfd, buf, CBUF_SIZE) < 0) {
        std::cerr << "Error: can't write message to socket!" << std::endl;
        throw EWrite();
    }
}

void TSocket::Write(std::string msg, int size) {
    char buf[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * sizeof(char));
    
    memcpy(buf, msg.c_str(), msg.size());
        
    if (write(sockfd, buf, size) < 0) {
        std::cerr << "Error: can't write message to socket!" << std::endl;
        throw EWrite();
    }
}

void TSocket::Send(const std::string & msg, size_t size) {
    if (size > CBUF_SIZE) {
        std::cerr << "Size of message is bigger than buffer" << std::endl;
        throw ESend();
    }
    char buf[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * sizeof(char));

    memcpy(buf, msg.c_str(), size);
    if (send(sockfd, buf, CBUF_SIZE * sizeof(char), MSG_NOSIGNAL) < 0) {
        std::cerr << "Error: can't send message to socket!" << std::endl;
        throw ESend();
    }
}

void TSocket::Send(const std::string & msg) {
    return Send(msg, msg.size());
}

std::string TSocket::Recv(size_t size) {
    if (size > CBUF_SIZE) {
        std::cerr << "Size of recved message is bigger than buffer!" << std::endl;
        throw ERecv();
    }
    char buf[CBUF_SIZE + 1];
    memset(buf, 0, CBUF_SIZE * sizeof(char));

    size_t totalReceved = 0;
    while(totalReceved != size) {
        size_t receved = recv(sockfd, buf + totalReceved, size, 0);
        
        if (receved < 0) {
            std::cerr << "Error: can't recv message from socket!" << std::endl;
            throw ERecv();
        }

        totalReceved += receved;
    }
    buf[totalReceved] = '\0';
    return std::string(buf);
}

std::string TSocket::Recv() {
    return Recv(CBUF_SIZE);
}


std::string TSocket::Read() {
    char buf[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * sizeof(char));
    
    if ((errcode = read(sockfd, buf, CBUF_SIZE)) < 0) {
        std::cerr << "Error: can't read message from socket!" << std::endl;
        std::cerr << "Error: errcode = " << errcode << std::endl;
        std::cerr << "Error: errno   = " << errno << std::endl;
        throw ERead();
    }

    return std::string(buf);
}

std::string TSocket::getIp() {
    char ipAddr[CBUF_SIZE];
    memset(ipAddr, 0, CBUF_SIZE * sizeof(char));
    
    inet_ntop(AF_INET, (in_addr *) &(addr.sin_addr), ipAddr, CBUF_SIZE);
    
    return std::string(ipAddr);
}
