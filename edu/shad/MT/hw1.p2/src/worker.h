/**
    @brief TThreadWorker and its environment.

    Each worker environment has common message
    queue with tasks (GET, PARSE, POISON), log
    messages queue for dumping pages and 
    common set for check twice downloading.

    @author Aman Orazaev
*/
#pragma once

#include <memory>
#include <string>

#include <messages.h>
#include <queue.h>
#include <set.h>



typedef std::shared_ptr<TSynchronizedQueue<TTaskMessage> >
        TMsgQueueHolder;

typedef std::shared_ptr<TSynchronizedSet<std::string> >
        TStringSetHolder;


/**
    @brief worker thread environment.

    TODO: add downloader, parser, and handler factory.
*/
struct TWorkerEnvironment {
    TMsgQueueHolder taskQueue;
    TMsgQueueHolder logQueue;
    TStringSetHolder downloadedUrls;

    /// FIXME(orazaev@): add downloader and parser here.
    /// FIXME(orazaev@): add message handlers factory.

    TWorkerEnvironment()
        : taskQueue(new TSynchronizedQueue<TTaskMessage>())
        , logQueue(new TSynchronizedQueue<TTaskMessage>())
        , downloadedUrls(new TSynchronizedSet<std::string>())
    { }
};



/**
    @brief run functor for std::thread.
    Process messages until get poisoned message.
*/
class TThreadWorker {
public:
    TThreadWorker(const TWorkerEnvironment& env)
        : env_(env) { }

    void operator()() {
        bool alive = true;
        while (alive) {
            TTaskMessage message = env_.taskQueue->Take();
            switch (message.GetType()) {
                case T_GET:
                    ProcessGetMessage(message);
                    break;
                case T_PARSE:
                    ProcessParseMessage(message);
                    break;
                case T_POISON:
                    alive = false;
                    ProcessPoisonedMessage();
                    break;
                case T_LOG:
                    ProcessLogMessage(message);
                    break;
            };
        }
    }

    void ProcessGetMessage(const TTaskMessage& message) {
        printf("GET: %s\n", message.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin get message = '%s'", message.GetData()->c_str());
        TTaskMessage logTask(buffer, T_LOG);
        env_.taskQueue->Put(logTask);

        sprintf(buffer, "Parsing message = '%s'", message.GetData()->c_str());
        TTaskMessage parseTask(buffer, T_PARSE);
        env_.taskQueue->Put(parseTask);
    }

    void ProcessParseMessage(const TTaskMessage& message) {
        printf("PARSE: %s\n", message.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin parse message = '%s'", message.GetData()->c_str());
        TTaskMessage logTask(buffer, T_LOG);
        env_.taskQueue->Put(logTask);
    }

    void ProcessPoisonedMessage() {
        printf("POISON:\n");

        TTaskMessage poisonedTask("", T_POISON);
        env_.taskQueue->Put(poisonedTask);
    }

    void ProcessLogMessage(const TTaskMessage& message) {
        printf("LOG: %s\n", message.GetData()->c_str());
    }


private:
    TWorkerEnvironment env_;
};


/**
    @brief wrapper for std::thread constructor.
*/
void StartWorker(const TWorkerEnvironment& env) {
    TThreadWorker worker(env);
    worker();
}
