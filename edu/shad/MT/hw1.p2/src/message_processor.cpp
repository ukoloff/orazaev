#include <message_processor.h>

void TGetMessageHandler::Process(const TTaskMessage& msg) {
    TStringHolder url = msg.GetUrl();
    printf("[%d] GET: %s, get_queue.size = %d\n", env_.thread_number, url->c_str(), env_.taskQueue->Size());

    TStringHolder html = std::make_shared<std::string>(env_.downloader->GetUrl(*url));
    TTaskMessage logTask(url, html, T_LOG, msg.GetDepth());
    env_.logQueue->Put(logTask);


    if (msg.GetDepth() + 1 <= env_.maxDownloadDepth) {
        TTaskMessage parseTask(url, html, T_PARSE, msg.GetDepth());
        env_.resultQueue->Put(parseTask);
    }
}

void TParseMessageHandler::Process(const TTaskMessage& msg) {
    std::set<std::string> links = TLinkParser::ParseText(msg);

    for (auto link : links) {
        if (env_.downloadedUrls->Insert(link)) {
            printf("[%d] Link(%d): '%s', parse_queue.size = %d\n", env_.thread_number, msg.GetDepth() + 1, link.c_str(), env_.taskQueue->Size());
            TTaskMessage getTask(link, T_GET, msg.GetDepth() + 1);
            env_.resultQueue->Put(getTask);
        }
    }
}

void TPoisonMessageHandler::Process(const TTaskMessage&) {
    printf("[%d] POISON:\n", env_.thread_number);

    TTaskMessage poisonedTask(T_POISON);
    //env_.taskQueue->Clear();
    env_.taskQueue->Put(poisonedTask);
    env_.alive = false;
}

std::atomic<size_t> TLogMessageHandler::numberOfDumpedPages(0);

void TLogMessageHandler::Process(const TTaskMessage& msg) {
    printf("[%d] LOG: '%s', log_queue.size = %d\n", env_.thread_number, msg.GetUrl()->c_str(), env_.logQueue->Size());
    TLogHolder log = env_.log;

    *log << "URL: " << *msg.GetUrl() << "\n";
    *log << "DEPTH: " << msg.GetDepth() << "\n";
    if (msg.GetHtml() != 0) {
        *log << "DUMP:\n\n";
        *log << *msg.GetHtml();
    } else {
        *log << "ERROR: Bad url.\n";
    }
    *log << "\n\n\n";

    ++numberOfDumpedPages;

    if (env_.taskQueue->Size() == 0
        && env_.resultQueue->Size() == 0
        && env_.logQueue->Size() == 0
        && numberOfDumpedPages == env_.downloadedUrls->Size())
    {
        TTaskMessage poisonedTask(T_POISON);
        env_.taskQueue->Put(poisonedTask);
        env_.alive = false;
    }
}

void TMsgProcessor::Process(
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

