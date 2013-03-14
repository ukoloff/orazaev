/**
    @brief Thread messages description and thread environment.

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


    If you add new message type, you need define
    it's handler class also and add it to TMsgProcessor.


    @author Aman Orazaev
*/
#pragma once

#include <memory>
#include <string>

#include <queue.h>
#include <set.h>
#include <page_downloader.h>

/** @brief Message type for TTaskMessage. */
enum TMessageType {T_GET, T_PARSE, T_POISON, T_LOG};

typedef std::shared_ptr<std::string> TStringHolder;



/**
    class TTaskMessage.
    @brief queued messages for std::threads.

    pUrl: shared pointer to url.
    pHtml: shared pointer to downloaded page.
    type: \in {T_GET, T_PARSE, T_POISON, T_LOG}, parse, get message
          or temenate thread.

    TODO: add graph detph.
*/
class TTaskMessage {
public:
    TTaskMessage(
        const TStringHolder& pUrl,
        const TStringHolder& pHtml,
        const TMessageType& type)
        : pUrl(pUrl)
        , pHtml(pHtml)
        , type(type)
    { }

    TTaskMessage(
        const std::string& url,
        const std::string& html,
        const TMessageType& type)
        : pUrl(std::make_shared<std::string>(url))
        , pHtml(std::make_shared<std::string>(html))
        , type(type)
    { }

    TTaskMessage(
        const std::string& url,
        const TMessageType& type)
        : pUrl(std::make_shared<std::string>(url))
        , pHtml(NULL)
        , type(type)
    { }

    TTaskMessage(const TMessageType& type)
        : pUrl(NULL)
        , pHtml(NULL)
        , type(type)
    { }


    inline const TStringHolder& GetUrl() const { return pUrl; }
    inline const TStringHolder& GetHtml() const { return pHtml; }
    inline const TMessageType& GetType() const { return type; }

private:
    const TStringHolder pUrl;
    const TStringHolder pHtml;
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

    /** To identifying threads. Default = 0.*/
    int thread_number;

    TWorkerEnvironment()
        : taskQueue(new TSynchronizedQueue<TTaskMessage>())
        , logQueue(new TSynchronizedQueue<TTaskMessage>())
        , downloadedUrls(new TSynchronizedSet<std::string>())
        , alive(true)
        , downloader(0)
        , thread_number(0)
    { }
};



