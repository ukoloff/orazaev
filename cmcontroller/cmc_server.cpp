#include "cmc_server.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>


// request format is:
//{+?} [host name][DD][target name][.cluster]
static const std::string DD = ": ";

// TCMServer base functions description
TCMServer::TCMServer(int portno) 
    : data()
    , listener(portno) 
    , isListening(0)
    , operatorList() {
    listener.setParent(this);
}

void TCMServer::operateConnection(TSocket conn) {
    std::cout << "operation connection from " << conn.getIp() << std::endl;
    TThreadOperator* to = new TThreadOperator(conn, &data);
    operatorList.push_back(to);
    std::cout << "Create operator" << std::endl;
    to->Create();
    std::cout << "Creation is ok" << std::endl;
    to->Detach();
}

void TCMServer::startListen() {
    if (!isListening) {
        std::cout << "Creating listen thread" << std::endl;
        listener.Create();
        std::cout << "Creating is ok" << std::endl;
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
    std::cout << "Constructor get connection from " << connection.getIp() << std::endl;
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

    std::cout << "Got request " << request;
    
    return request;
}

std::string TCMServer::TThreadOperator::getRequestHost(std::string r) {
    int pos = r.find(DD);
    if (pos != std::string::npos)
        r.erase(pos, r.size() - pos);
    else
        r = "Unknown";

    std::cout << "Host: " << r << std::endl;
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

    std::cout << "Name: " << r << std::endl;
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

    std::cout << "Clus: " << r << std::endl;
    return r;
}

void TCMServer::TThreadOperator::addToMap(std::string request) {
    (*pdata)[request]++;
    (*pdata)[getRequestTargetName(request)]++;
    std::string cluster = getRequestCluster(request);
    std::cout << "Cluster: '" << cluster << "'" << std::endl;
    if (cluster != "") {
        (*pdata)[getRequestHost(request) + DD + getRequestTargetName(request)]++;
        (*pdata)[getRequestTargetName(request) + cluster]++;
    }
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
    std::cout << "Operator is running" << std::endl;
    std::string req = getRequest();
    std::string ans = operateRequest(req);
    // Ans
    sendAns(ans);
    connection.Close();
}


// TCMServer::TThreadListener description
// Just listen and create ThreadOperator on accept
TCMServer::TThreadListener::TThreadListener(int portno) 
    : listenSocket(portno) 
    , isStop(0) {
    std::cout << "Listen socket with port: " << portno << std::endl;
}

void TCMServer::TThreadListener::stop() {
    isStop = 1;
}

void TCMServer::TThreadListener::run() {
    listenSocket.Bind();
    listenSocket.Listen();

    while(!isStop) {
        std::cout << "Listening..." << std::endl;
        TSocket conn = listenSocket.Accept();
        conn.setCloseable(false);
        parent->operateConnection(conn);
    }

}

inline void TCMServer::TThreadListener::setParent(TCMServer* Parent) {
    parent = Parent;
}

