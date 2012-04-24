/*
 *	POSIX tcp socket wrapper.
 *
 *	by AOrazaev.
 *
 *	gmail me: AOrazaev[at]gmail.com
 */
#include <string>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <netdb.h>

class TSocket {
	TSocket(const TSocket& copy);
    TSocket& operator= (TSocket&);
    TSocket();

    int sockfd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
public:
	TSocket(std::string hostname, int portno);
    virtual ~TSocket();

    void write(std::string msg);
    std::string read();
    
    //exceptions
    class ESocket {};
    class EBind    : public ESocket {};
    class EConnect : public ESocket {};
    class ERead    : public ESocket {};
    class EWrite   : public ESocket {};

    //consts
    static const int CBUF_SIZE = 256;
};


