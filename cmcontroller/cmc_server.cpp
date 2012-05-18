#include "cmc_server.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>


// request format is:
// [host name][DD][target name][.cluster]
static const std::string DD = ": ";

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

std::string TCMServer::TThreadOperator::getRequest() {
    std::string request;

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
    
    return request;
}

std::string TCMServer::TThreadOperator::getRequestHost(std::string r) {
    int pos = r.find(DD);
    if (pos != std::string::npos)
        r.erase(pos, r.size() - pos);
    else
        r = "Unknown";

    return r;
}

std::string TCMServer::TThreadOperator::getRequestTargetName(std::string r) {
    int pos = r.find(DD);
    if (pos != std::string::npos)
        r.erase(0, pos + 2);
    else
        return "Unknown";

    pos = r.rfind('.');
    if (pos != std::string::npos)
        r.erase(pos + 1, r.size() - pos - 1);

    return r;
}

std::string TCMServer::TThreadOperator::getRequestCluster(std::string r) {
    int pos = r.find(DD);
    if (pos != std::string::npos)
        r.erase(0, pos + 2);
    else
        return "Unknown";

    pos = r.rfind('.');
    if (pos != std::string::npos)
        r.erase(0, pos + 1);
    else
        return "";

    return r;
}

void TCMServer::TThreadOperator::addToMap(std::string request) {
    (*pdata)[request]++;
    (*pdata)[getRequestTargetName(request)]++;
    (*pdata)[getRequestHost(request) + DD + getRequestTargetName(request)]++;
    std::string cluster = getRequestCluster(request);
    if (cluster != "")
        (*pdata)[getRequestTargetName(request) + cluster]++;
}

// It's 2 operation availiable:
// + --- add 1 to map[request]
// ? --- get value of map[request]
//
// It can be new operation description here
std::string TCMServer::TThreadOperator::operateRequest(std::string request) {
    int count = 0;
    std::string ans;


    switch(request[0]) {
        case '+':
            request.erase(0, 1);
            if (request[0] == ' ')
                request.erase(0, 1);
            
            mutexLock();
            addToMap(request);
            mutexUnlock();

            ans = its(count);
        break;
        case '?':
            request.erase(0, 1);
            if (request[0] == ' ')
                request.erase(0, 1);

            mutexLock();
            count = (*pdata)[request];
            mutexUnlock();

            ans = its(count);
        break;
    }
    

    return ans;
}

void TCMServer::TThreadOperator::sendAns(const std::string& ans) {
    try {
        connection.Write(ans);
    }
    catch(TSocket::EWrite) {
        std::cerr << "Can't answer to client " << connection.getIp() << std::endl;
        exit(1);
    }   
}

void TCMServer::TThreadOperator::run() {
    // Read
    // Operate block
    std::string ans = operateRequest(getRequest());
    // Ans
    sendAns(ans);
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

