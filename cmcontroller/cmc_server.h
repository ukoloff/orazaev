#ifndef CMC_SERVER_H
#define CMC_SERVER_H

#include "../util/socket/TSocket.h"
#include "../util/thread/TThread.h"
#include "../util/logger/logger.h"

#include <string>
#include <map>
#include <list>
#include <memory>

class TCMServer {
    // Operator class
    class TThreadOperator : public TThread {
        TSocket connection;
        std::map<std::string, int>* pdata;
        TCMServer* parent;

        TThreadOperator();

        inline std::string its(int);
        inline std::string getRequest();
        inline std::string getRequestHost(std::string );
        inline std::string getRequestTargetName(std::string );
        inline std::string getRequestCluster(std::string );
        inline void addToMap(std::string request);

        inline std::string operateRequest(std::string request);
        inline void sendAns(const std::string& ans);
    public:
        TThreadOperator(TSocket conn, std::map<std::string, int>* pd);
        virtual void run();

        inline void setParent(TCMServer* p) { parent = p; }
    };

    // Listener class
    class TThreadListener : public TThread {
        TSocket listenSocket;
        int isStop;
        TCMServer* parent;
    public:
        TThreadListener(int portno);
        virtual void run();

        void stop();
        inline void setParent(TCMServer*);
    };

    std::map<std::string, int> data;
    TThreadListener listener;
    bool isListening;
    std::list<TThreadOperator*> operatorList;
    
    std::auto_ptr<TLogger> msgLog;
    std::auto_ptr<TLogger> mainLog;
    inline void operateConnection(TSocket conn);
public:
    TCMServer(int portno);
    
    void startListen();
    void stopListen();

    inline void setMsgLog(const std::string& fname) {
        msgLog = std::auto_ptr<TLogger>(new TLogger(fname));
        msgLog->clear();
    }

    inline void setMainLog(const std::string& fname) {
        mainLog = std::auto_ptr<TLogger>(new TLogger(fname));
        mainLog->clear();
    }
};

#endif
