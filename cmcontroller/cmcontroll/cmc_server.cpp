#include "cmc_server.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>


// request format is:
//{+?} [host name][DD][target name][.cluster]
// need new command to dump, @ filename
static const std::string DD = ": ";
static const int dump_buf_size = CM_DUMP_BUF_SIZE;

// TCMServer base functions description
TCMServer::TCMServer(int portno) 
    : data()
    , listener(portno) 
    , isListening(0)
    , operatorList()
    , msgLog(0)
    , mainLog(0) {
    listener.setParent(this);
}

void TCMServer::operateConnection(TSocket conn) {
    if (mainLog.get()) *mainLog << "get connection from " << conn.getIp() << log::endl;
    TThreadOperator* to = new TThreadOperator(conn, &data);
    to->setParent(this);

    to->Create();
    operatorList.push_back(to);
}

void TCMServer::startListen() {
    if (!isListening) {
        if (mainLog.get()) *mainLog << "start listening." << log::endl;
        listener.Create();
        listener.Detach();

        isListening = 1;
    }

    while(isListening) {
        while(!operatorList.empty()) {
            TThreadOperator* po = operatorList.front();
            po->Join();
            operatorList.pop_front();
            delete po;
        }
    }
}

void TCMServer::stopListen() {
    if (isListening) {
        listener.stop();

        isListening = 0;
    }
}

int TCMServer::dumpData(const std::string& fname) {
    FILE* f = fopen(fname.c_str(), "wb");
    
    if (f == 0) {
        std::cerr << "Can't open file '" << fname << "' with wb mode" << std::endl;
        return 1;
    }

    char buf[dump_buf_size * sizeof(char)];
    for (std::map<std::string, int>::const_iterator it = data.begin(); it != data.end(); it++) {
        memset(buf, 0, dump_buf_size * sizeof(char));
        memcpy(buf, it->first.c_str(), it->first.size() * sizeof(char));
        fwrite(buf, sizeof(char), dump_buf_size, f);
        fwrite(&(it->second), 1, sizeof(int), f);
    }

    fclose(f);
    
    return 0;
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

    while (request[request.size() - 1] == '\n' || request[request.size() - 1] == '\r')
        request.resize(request.size() - 1);

    mutexLock();
    if (parent->msgLog.get()) {
        *(parent->msgLog) << connection.getIp() << " got request '" << request;
        *(parent->msgLog) << "'" << log::endl;
    }
    mutexUnlock();
 
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

    pos = r.find('.');
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

    pos = r.find('.');
    if (pos != std::string::npos)
        r.erase(0, pos + 1);
    else
        return "";

    return r;
}

void TCMServer::TThreadOperator::addToMap(std::string request) {
    (*pdata)[request]++;

    std::string name = getRequestTargetName(request);
    if (name != "Unknown")
        (*pdata)[getRequestTargetName(request)]++;
    else return;

    std::string cluster = getRequestCluster(request);
    if (cluster == "Unknown") return;

    std::string host;
    if (cluster != "") {
        host = getRequestHost(request);
        if (host != "Unknown") 
            (*pdata)[host + DD + name]++;
        else return;

        (*pdata)[name + cluster]++;
    }

/*
    if (parent->msgLog.get()) {
        *(parent->msgLog) << "addToMap()";
        *(parent->msgLog) << "\n    + '" << request;
        *(parent->msgLog) << "\n    + '" << host;
        *(parent->msgLog) << "\n    + '" << name;
        *(parent->msgLog) << "\n    + '" << cluster << log::endl;
    }
*/
        
}

// It's 2 operation availiable:
// + --- add 1 to map[request]
// ? --- get value of map[request]
// d --- dump map to file as arg
//
// It can be new operation description here
std::string TCMServer::TThreadOperator::operateRequest(std::string request) {
    std::string ans;


    switch(request[0]) {
        case '+':
            request.erase(0, 1);
            if (request[0] == ' ')
                request.erase(0, 1);
            
            mutexLock();
            addToMap(request);
            mutexUnlock();

            ans = "0";
        break;
        case '?':
        {
            int count = 0;

            request.erase(0, 1);
            if (request[0] == ' ')
                request.erase(0, 1);

            mutexLock();
            count = (*pdata)[request];
            mutexUnlock();

            ans = its(count);
        }
        break;
        case 'd':
            request.erase(0, 1);
            if (!request.empty() && request[0] == ' ')
                request.erase(0, 1); 

            if (request.empty()) 
                return std::string("1");

            mutexLock();
            if (parent->dumpData(request) == 0)
                ans = "0";
            else
                ans = "1";
            mutexUnlock();
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
    //if (parent->mainLog.get()) *(parent->mainLog) << "Operate connection " << connection.getIp() << log::endl;
    std::string req = getRequest();
    std::string ans = operateRequest(req);
    sendAns(ans);

    connection.Close();
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
        if (parent->mainLog.get()) *(parent->mainLog) << "Start listening..."  << log::endl;
        TSocket conn = listenSocket.Accept();
        conn.setCloseable(false);
        parent->operateConnection(conn);
    }

}

inline void TCMServer::TThreadListener::setParent(TCMServer* Parent) {
    parent = Parent;
}
