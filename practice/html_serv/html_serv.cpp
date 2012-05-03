#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

#include "../../util/thread/TThread.h"
#include "../../util/thread/TMutex.h"
#include "../../util/socket/TSocket.h"

void start_server(TSocket& connection) {

    std::string buf = "";
    buf = connection.Read();
    char c[1024]; 
    
    std::cout << "Got Request:" << std::endl;
    std::cout << "'" << buf << "'" << std::endl;
    if (buf.size() == 0) return;

    buf = "";
    while(std::cin.good()) {
        std::cin.getline(c, 1024);
        buf += std::string(c) + "\n";
    }

/*
    sprintf(c, "HTTP/1.1 200 OK\n\
Server: nginx\n\
Date: Thu, 25 Feb 2012 12:31:25 GMT\n\
Last-Modified: Tue, 12 Jan 2012 15:29:06 GMT\n\
Content-Type: text/html; charset=utf-8\n\
Content-Length: %d\n\
Transfer-Encoding: chunked\n\
Connection: keep-alive\n\n", buf.size());
*/

    sprintf(c, "HTTP/1.1 200 OK\n\
Content-Type: text/html; charset=utf-8\n\
Content-Length: %d\n\n", buf.size());
/*
    sprintf(c, "HTTP/1.1 200 OK\n\
Content-Type: text/html; charset=utf-8\n\
\n");
*/
    std::string head = std::string(c);

    buf = head + buf + "\n";

    std::cout << "Sending: " << std::endl << buf << std::endl;
    connection.Write(buf, buf.size());
}

int main(int argc, char** argv) {
    if ( argc < 2 ) {
        std::cerr << "Error: You must set port argument after program name!" << std::endl;
        return 1;
    }

    int portno = atoi(argv[1]);
    
    try {
        TSocket s(portno);
        s.Bind();
        s.Listen();
        TSocket connection = s.Accept();
        while(true)
            start_server(connection);
    }
    catch (TSocket::ESocket) {
        std::cerr << "Error: ESocket catched --- something goes wrong." << std::endl;

        return 1;
    }

    return 0;
}
