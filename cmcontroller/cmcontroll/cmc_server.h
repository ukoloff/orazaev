#ifndef CMC_SERVER_H
#define CMC_SERVER_H

#include<cmcontroller/cmdefinitions.h>
#include<util/thread/TThread.h>
#include<util/logger/logger.h>

#include<string>
#include<map>
#include<list>
#include<memory>

class TCMServer {
    // Operator class
    class TThreadOperator : public TThread {
        TCMSocket connection;
        std::map<std::string, int>* pdata;
        TCMServer* parent;

        TThreadOperator();

        inline std::string its(int);
        inline std::string getRequest();
        inline std::string getRequestHost(std::string );
        inline std::string getRequestTargetName(std::string );
        inline std::string getRequestCluster(std::string );
        inline void stripAddRequest(std::string&);
        inline void addToMap(std::string request);

        inline std::string operateRequest(std::string request);
        inline void sendAns(const std::string& ans);
    public:
        TThreadOperator(TCMSocket conn, std::map<std::string, int>* pd);
        virtual void run();

        inline void setParent(TCMServer* p) { parent = p; }
    };


    // Listener class
    class TThreadListener : public TThread {
        TCMSocket listenSocket;
        TCMServer* parent;
    public:
        TThreadListener();
        virtual void run();

        inline void setParent(TCMServer*);
        inline void setSocket(const TCMSocket& ls) {
            listenSocket = ls;
        }
    };


    std::map<std::string, int> data;

    TCMSocket listenSocket;
    TThreadListener listener;
    bool isListening;
    std::list<TThreadOperator*> operatorList;
    
    std::auto_ptr<TLogger> msgLog;
    std::auto_ptr<TLogger> mainLog;

    inline void operateConnection(TCMSocket conn);
    int dumpData(const std::string& fname);

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
