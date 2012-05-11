#include "cmc_server.h"

TCMServer::TCMServer(int portno) \
    : data()
    , listener(portno) 
    , isListening(0) {
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
    if (isLisetning) {
        listener.stop();

        isListening = 0;
    }
}

TCMServer::TThreadOperator::TThreadOperator(TSocket conn, map<std::string, int>* pd) \
    : connection(conn) \
    , pdata(pd) {
}

void TCMServer::TTHreadOperator::run() {
    //Read

    //Operate

    //Ans
}

TCMServer::TThreadListener::TThreadListener(int portno) \
    : listenSocket(portno) \
    , isStop(0) {
}

void TCMServer::TThreadListener::Stop() {
    isStop = 1;
}

void TCMServer::TThreadListener::run() {
    listenSocket.Bind();
    listenSocket.Listen();

    TSocket conn;
    while(!isStop) {
        conn = listenSocket.Accept();
        operateConnection(conn);
    }

    listeSocket.Close();
}
