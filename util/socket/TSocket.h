/*
 *	POSIX tcp socket wrapper.
 *
 *	by AOrazaev.
 *
 *	gmail me: AOrazaev[at]gmail.com
 */
#ifndef TSOCKET_H
#define TSOCKET_H
#include <string>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netdb.h>

class TSocket {
    int sockfd;
    struct sockaddr_in addr;

public:
    TSocket() {}
	TSocket(const std::string& hostname, const int& portno);
    TSocket(const int& Sockfd, const struct sockaddr_in& Addr);
    TSocket(const int& portno);

    virtual ~TSocket(); 

    void Connect();
    void Listen();
    void Bind();
    TSocket Accept();

    void Write(std::string msg);
    void Write(std::string msg, int size);
    std::string Read();

    std::string getIp();

    //exceptions
    class ESocket {};
    class EBind    : public ESocket {};
    class EConnect : public ESocket {};
    class EListen  : public ESocket {};
    class EAccept  : public ESocket {};
    class ERead    : public ESocket {};
    class EWrite   : public ESocket {};

    //consts
    static const int CBUF_SIZE = 1024;
};

#endif
