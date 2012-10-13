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
#include <util/TError.h>

template <size_t BUF_SIZE>
class TAbstarctSocket {
public:
    TAbstarctSocket() {};

    virtual ~TAbstarctSocket() {};

    virtual void Listen() = 0;
    virtual void Bind() = 0;
    virtual void Close() = 0;

    virtual void Send(const std::string& msg) = 0;
    virtual void Send(const std::string& msg, size_t size) = 0;
    virtual std::string Recv() = 0;
    virtual std::string Recv(size_t size) = 0;

};

// Include real sockets
#include"tcpsocket.h"

// Test socket with 1 MB buffer.
typedef TTCPSocket<1024> TMbTCPSocket;

#endif
