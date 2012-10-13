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

#include <util/TError.h>

// TCP socket
class TTCPSocket {
    int sockfd;
    struct sockaddr_in addr;

public:
    TTCPSocket() {}
	TTCPSocket(const std::string& hostname, const int& portno);
    TTCPSocket(const int& Sockfd, const struct sockaddr_in& Addr);
    TTCPSocket(const int& portno);

    virtual ~TTCPSocket(); 

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

    //exception
    class ESocket : public TError {
    public:
        ESocket(const std::string& m) throw()
            : TError(m)
        { }
    };

    //consts
    static const int CBUF_SIZE = 1024;
};

#endif
