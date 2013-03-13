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

    void Process(const TTaskMessage& msg);
};



/** @brief T_PARSE message handler. */
class TParseMessageHandler : public TMessageHandler {
public:
    TParseMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg);
};



/** @brief T_POISON message handler. */
class TPoisonMessageHandler : public TMessageHandler {
public:
    TPoisonMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage&);
};



/** @brief T_LOG message handler. */
class TLogMessageHandler : public TMessageHandler {
public:
    TLogMessageHandler(TWorkerEnvironment& env)
        : TMessageHandler(env)
    { }

    void Process(const TTaskMessage& msg);
};



/** @brief Message handlers trigger. */
class TMsgProcessor {
public:
    static void Process(
            const TTaskMessage& msg,
            TWorkerEnvironment& env);

private:
    TMsgProcessor() = delete;
    ~TMsgProcessor() = delete;
};



/**
    @brief run funcion for std::thread.
    Process messages until get poisoned message.
*/
class TThreadWorker {
public:
    TThreadWorker(const TWorkerEnvironment& env)
        : env_(env) { }

    void Run();
private:
    TWorkerEnvironment env_;
};


/**
    @brief wrapper for std::thread constructor.
*/
void StartWorker(const TWorkerEnvironment& env);

