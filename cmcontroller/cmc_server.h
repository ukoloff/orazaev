#ifndef CMC_SERVER_H
#define CMC_SERVER_H

#include "../util/socket/TSocket.h"
#include "../util/thread/TThread.h"

#include <string>

class TCMServer {
    // Operator class
    class TThreadOperator() :: public TThread {
        TSocket connection;
        map<std::string, int>* pdata;

        TThreadOperator();
    public:
        TThreadOperator(TSocket conn, map<std::string, int>* pd);
        virtual void run();
    };

    // Listener class
    class TThreadListener() :: public TThread {
        TSocket listenSocket;
        int isStop;
    public:
        TThreadListener(int portno);
        virtual void run();

        void stop();
    };

    map<std::string, int> data;
    TThreadListener listener;
    bool isListening;
    void operateConnection(TSocket conn);
public:
    TCMServer(int portno);
    
    void startListen();
    void stopListen();
};

#endif
