#ifndef __TSOCKET__
#define __TSOCKET__

#include "TSocket.h"
#include <iostream>
#include <cstring>


TSocket::TSocket(std::string hostname, int portno) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cerr << "Error: can't create socket!" << std::endl;
        throw ESocket();
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(hostname.c_str());
    serv_addr.sin_port = htons(portno);
}

void TSocket::write(std::string msg) {
    
}

std::string TSocket::read() {

}

#endif
