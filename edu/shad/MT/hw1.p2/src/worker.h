/**
    @brief TThreadWorker and its environment.

    Each worker environment has common message
    queue with tasks (GET, PARSE, POISON), log
    messages queue for dumping pages and 
    common set for check twice downloading.


    Also has TMsgProcessor class. Thread should call:
        TMsgProcessor::Process(msg, env);
    to process msg. TMsgProcessor will call message
    type handler.

    If you add new message type, you need define
    it's handler class also and add it to TMsgProcessor.


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

    /** alive = true, until wasn't processed poisoned message. */
    bool alive;

    /// FIXME(orazaev@): add downloader and parser here.
    /// FIXME(orazaev@): add message handlers factory.

    TWorkerEnvironment()
        : taskQueue(new TSynchronizedQueue<TTaskMessage>())
        , logQueue(new TSynchronizedQueue<TTaskMessage>())
        , downloadedUrls(new TSynchronizedSet<std::string>())
        , alive(true)
    { }
};



/** @brief Abstract message handler. */
class TMessageHandler {
protected:
    TMessageHandler(TWorkerEnvironment& env)
        : env_(env)
    { }
    ~TMessageHandler() { }

protected:
    void Process(TTaskMessage&);

protected:
    TWorkerEnvironment& env_;
};



/** @brief T_GET message handler. */
class TGetMessageHandler : public TMessageHandler {
public:
    TGetMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg) {
        printf("GET: %s\n", msg.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin get message = '%s'", msg.GetData()->c_str());
        TTaskMessage logTask(buffer, T_LOG);
        env_.taskQueue->Put(logTask);

        sprintf(buffer, "Parsing message = '%s'", msg.GetData()->c_str());
        TTaskMessage parseTask(buffer, T_PARSE);
        env_.taskQueue->Put(parseTask);
    }
};



/** @brief T_PARSE message handler. */
class TParseMessageHandler : public TMessageHandler {
public:
    TParseMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg) {
        printf("PARSE: %s\n", msg.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin parse message = '%s'", msg.GetData()->c_str());
        TTaskMessage logTask(buffer, T_LOG);
        env_.taskQueue->Put(logTask);
    }
};



/** @brief T_POISON message handler. */
class TPoisonMessageHandler : public TMessageHandler {
public:
    TPoisonMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage&) {
        printf("POISON:\n");

        TTaskMessage poisonedTask("", T_POISON);
        env_.taskQueue->Put(poisonedTask);
        env_.alive = false;
    }
};



/** @brief T_LOG message handler. */
class TLogMessageHandler : public TMessageHandler {
public:
    TLogMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg) {
        printf("LOG: %s\n", msg.GetData()->c_str());
    }
};



/** @brief Message handlers trigger. */
class TMsgProcessor {
public:
    static void Process(
        const TTaskMessage& msg,
        TWorkerEnvironment& env)
    {
        switch(msg.GetType()) {
            case T_GET:
                TGetMessageHandler(env).Process(msg);
                break;
            case T_PARSE:
                TParseMessageHandler(env).Process(msg);
                break;
            case T_POISON:
                TPoisonMessageHandler(env).Process(msg);
                break;
            case T_LOG:
                TLogMessageHandler(env).Process(msg);
                break;
        };
    }

private:
    TMsgProcessor() = delete;
    ~TMsgProcessor() = delete;
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
        while (env_.alive) {
            TTaskMessage message = env_.taskQueue->Take();
            TMsgProcessor::Process(message, env_);
        }
    }

    void ProcessGetMessage(const TTaskMessage& message) {
    }

    void ProcessParseMessage(const TTaskMessage& message) {
    }

    void ProcessPoisonedMessage() {
    }

    void ProcessLogMessage(const TTaskMessage& message) {
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





