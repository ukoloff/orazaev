/**
    @brief Thread messages description, environment and message handlers.

    Need for thread message queue. Each message
    type should have its handler or processor.

    class TTaskMessage.
    Messages types:
        T_GET: crawler thread get command for
        download url. There are url link in string data.

        T_PARSE: thread parse command to parse links
        in page and add T_GET messages for this links
        in queue. There are page content in string data.

        T_POISON: thread terminate command.

        T_LOG: dump string data to log-dump file.
        One threaded, becouse work with out file.
        It means that that messages need one individual
        queue and one individual thread to dump pages.


    class TWorkerEnvironment.
    Each worker environment has common message
    queue with tasks (GET, PARSE, POISON), log
    messages queue for dumping pages and 
    common set for check twice downloading.


    class TMsgProcessor.
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

#include <queue.h>
#include <set.h>
#include <link_parser.h>
#include <page_downloader.h>

/** @brief Message type for TTaskMessage. */
enum TMessageType {T_GET, T_PARSE, T_POISON, T_LOG};

typedef std::shared_ptr<std::string> TStringHolder;

/**
    class TTaskMessage.
    @brief queued messages for std::threads.

    pData: url to get, or url content to parse.
    type: \in {T_GET, T_PARSE, T_POISON, T_LOG}, parse, get message
          or temenate thread.
*/
class TTaskMessage {
public:
    TTaskMessage(
        const TStringHolder& pData,
        const TMessageType& type)
        : pData(pData)
        , type(type)
    { }

    TTaskMessage(
        const std::string& s,
        const TMessageType& type)
        : pData(std::make_shared<std::string>(s))
        , type(type)
    { }


    inline const TMessageType& GetType() const {
        return type;
    }

    inline const TStringHolder& GetData() const {
        return pData;
    }

private:
    const TStringHolder pData;
    TMessageType type;
};



typedef std::shared_ptr<TSynchronizedQueue<TTaskMessage> > TMsgQueueHolder;
typedef std::shared_ptr<TSynchronizedSet<std::string> > TStringSetHolder;
typedef std::shared_ptr<TPageDownloader> TDownloaderHolder;


/**
    @brief worker thread environment.

    Use static function of TMsgProcessor to process messages.
    Use static function of TLinkParser to extract links from
    downloaded html.
*/
struct TWorkerEnvironment {
    TMsgQueueHolder taskQueue;
    TMsgQueueHolder logQueue;
    TStringSetHolder downloadedUrls;

    /** alive = true, until wasn't processed poisoned message. */
    bool alive;

    /**
        Page downloader are one for each thread.
        Should be created in run function.
    */
    TDownloaderHolder downloader;

    TWorkerEnvironment()
        : taskQueue(new TSynchronizedQueue<TTaskMessage>())
        , logQueue(new TSynchronizedQueue<TTaskMessage>())
        , downloadedUrls(new TSynchronizedSet<std::string>())
        , alive(true)
        , downloader(0)
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
