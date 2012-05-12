#include "cmc_server.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>


// TCMServer base functions description
TCMServer::TCMServer(int portno) 
    : data()
    , listener(portno) 
    , isListening(0) {
    listener.setParent(this);
}

void TCMServer::operateConnection(TSocket conn) {
    TThreadOperator to(conn, &data);
    to.Create();
    to.Detach();
}

void TCMServer::startListen() {
    if (!isListening) {
        listener.Create();
        listener.Detach();

        isListening = 1;
    }
}

void TCMServer::stopListen() {
    if (isListening) {
        listener.stop();

        isListening = 0;
    }
}


// TCMServer::TThreadOperator description
TCMServer::TThreadOperator::TThreadOperator(TSocket conn, std::map<std::string, int>* pd) 
    : connection(conn) 
    , pdata(pd) {
}

std::string TCMServer::TThreadOperator::its(int x) {
    char buf[100];
    memset(buf, 0, 100);
    sprintf(buf, "%d", x);
    return std::string(buf);
} 

void TCMServer::TThreadOperator::run() {
    // Read
    std::string request;
    std::string ans;
    int count;

    try {
        request = connection.Read();
    }
    catch (TSocket::ERead) {
        try {
            connection.Write("?");
        }
        catch (TSocket::EWrite) {
            std::cerr << "Connection problem." << std::endl;
            exit(1);
        }
    }
    
    // Operate block
    // It's 2 operation availiable:
    // + --- add 1 to map[request]
    // ? --- get value of map[request]
    //
    // It can be new operation description here
    switch(request[0]) {
        case '+':
            request.erase(0, 1);

            mutexLock();
            if ((*pdata)[request] == 0)
                count = (*pdata)[request] = 1;
            else
                count = (*pdata)[request] += 1;
            mutexUnlock();

            ans = its(count);
        break;
        case '?':
            request.erase(0, 1);

            mutexLock();
            count = (*pdata)[request];
            mutexUnlock();

            ans = its(count);
        break;
    }
    

    // Ans
    try {
        connection.Write(ans);
    }
    catch(TSocket::EWrite) {
        std::cerr << "Can't answer to client " << connection.getIp() << std::endl;
        exit(1);
    }
}


// TCMServer::TThreadListener description
// Just listen and create ThreadOperator on accept
TCMServer::TThreadListener::TThreadListener(int portno) 
    : listenSocket(portno) 
    , isStop(0) {
}

void TCMServer::TThreadListener::stop() {
    isStop = 1;
}

void TCMServer::TThreadListener::run() {
    listenSocket.Bind();
    listenSocket.Listen();

    while(!isStop) {
        parent->operateConnection(listenSocket.Accept());
    }

}

inline void TCMServer::TThreadListener::setParent(TCMServer* Parent) {
    parent = Parent;
}

