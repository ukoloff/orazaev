/**
    @brief TThreadWorker and its environment.

    Each worker environment has common message
    queue with tasks (GET, PARSE, POISON), log
    messages queue for dumping pages and 
    common set for check twice downloading.
*/
#pragma once
#include <memory>
#include <messages.h>
#include <queue.h>
#include <set.h>

typedef std::shared_ptr<TSynchronizedQueue<TTaskMessage> >
        TMsgQueueHolder;

typedef std::shared_ptr<TSynchronizedSet<std::string> >
        TStringSetHolder;

struct TWorkerEnvironment {
    TMsgQueueHolder taskQueue;
    TMsgQueueHolder logQueue;
    TStringSetHolder downloadedUrls;

    /// FIXME(orazaev@): add downloader and parser here.
    /// FIXME(orazaev@): add message handlers factory.
};

/**
    @brief run functor for std::thread.
    Process messages until get poisoned message.
*/
class TThreadWorker {
public:
    TThreadWorker(
            const std::shared_ptr<TSynchronizedQueue<TTaskMessage> >& messages)
    : messages(messages) { }

    void operator()() {
        bool alive = true;
        while (alive) {
            TTaskMessage message = messages->Take();
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
        TTaskMessage logTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_LOG);
        messages->Put(logTask);

        sprintf(buffer, "Parsing message = '%s'", message.GetData()->c_str());
        TTaskMessage parseTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_PARSE);
        messages->Put(parseTask);
    }

    void ProcessParseMessage(const TTaskMessage& message) {
        printf("PARSE: %s\n", message.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin parse message = '%s'", message.GetData()->c_str());
        TTaskMessage logTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_LOG);
        messages->Put(logTask);
    }

    void ProcessPoisonedMessage() {
        printf("POISON:\n");

        TTaskMessage poisonedTask(
            std::shared_ptr<std::string>(),
            T_POISON);
        messages->Put(poisonedTask);
    }

    void ProcessLogMessage(const TTaskMessage& message) {
        printf("LOG: %s\n", message.GetData()->c_str());
    }


private:
    std::shared_ptr<TSynchronizedQueue<TTaskMessage> > messages;
};
