#ifndef TTCPSOCKET_H
#define TTCPSOCKET_H

#include"socket.h"

#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <cstring>
#include <cerrno>
#include <memory>


template <size_t CBUF_SIZE>
class TTCPSocket : public TAbstarctSocket<CBUF_SIZE> {
public:
    TTCPSocket() {}
	TTCPSocket(const std::string& hostname, const int& portno);
    TTCPSocket(const int& Sockfd, const struct sockaddr_in& Addr);
    TTCPSocket(const int& portno);

    virtual ~TTCPSocket() {}; 

    void Connect();
    void Listen();
    void Bind();
    TTCPSocket Accept();
    void Close();

    void Send(const std::string& msg);
    void Send(const std::string& msg, size_t size);
    std::string Recv(size_t size);
    std::string Recv();

    void Write(const std::string& msg);
    void Write(const std::string& msg, size_t size);
    std::string Read();

    std::string getIp();


private:
    int sockfd;
    struct sockaddr_in addr;
};




/* ----------- Realization ----------- */




template <size_t CBUF_SIZE>
TTCPSocket<CBUF_SIZE>::TTCPSocket(const int& Sockfd, const struct sockaddr_in& Addr) 
    : sockfd(Sockfd)
    , addr(Addr) {
}




template <size_t CBUF_SIZE>
TTCPSocket<CBUF_SIZE>::TTCPSocket(const std::string& hostname, const int& portno) {
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




template <size_t CBUF_SIZE>
TTCPSocket<CBUF_SIZE>::TTCPSocket(const int& portno) {
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        throw ESocket("Error: can't create socket!");
    }

    memset(&addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(portno);
    this->addr.sin_addr.s_addr = INADDR_ANY;
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Connect() {
    if (connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw ESocket("Error: can't connect to host!");
    }   
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Listen() {
    if (listen(sockfd, 5) < 0) {
        throw ESocket("Error: can't set port as listening!");
    }
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Bind() {
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw ESocket("Error: can't bind socket!");
    }
}




template <size_t CBUF_SIZE>
TTCPSocket<CBUF_SIZE> TTCPSocket<CBUF_SIZE>::Accept() {
    struct sockaddr_in cli_addr;
    int conn;
    unsigned int tsize = sizeof(cli_addr);

    memset(&cli_addr, 0, tsize);
    conn = accept(sockfd, (struct sockaddr *) &cli_addr, &tsize);

    if (conn < 0) {
        throw ESocket("Error: can't accepting connection!");
    }
    
    return TTCPSocket<CBUF_SIZE>(conn, cli_addr);
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Close() {
        close(sockfd);
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Send(const std::string & msg, size_t size) {
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




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Send(const std::string & msg) {
    return Send(msg, msg.size());
}




template <size_t CBUF_SIZE>
std::string TTCPSocket<CBUF_SIZE>::Recv(size_t size) {
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




template <size_t CBUF_SIZE>
std::string TTCPSocket<CBUF_SIZE>::Recv() {
    return Recv(CBUF_SIZE);
}




template <size_t CBUF_SIZE>
std::string TTCPSocket<CBUF_SIZE>::Read() {
    return Recv();
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Write(const std::string& msg) {
    Send(msg);
}




template <size_t CBUF_SIZE>
void TTCPSocket<CBUF_SIZE>::Write(const std::string& msg, size_t size) {
    Send(msg, size);
}




template <size_t CBUF_SIZE>
std::string TTCPSocket<CBUF_SIZE>::getIp() {
    char ipAddr[CBUF_SIZE];
    memset(ipAddr, 0, CBUF_SIZE * sizeof(char));
    
    inet_ntop(AF_INET, (in_addr *) &(addr.sin_addr), ipAddr, CBUF_SIZE);
    
    return std::string(ipAddr);
}




#endif /* TTCPSOCKET_H */
