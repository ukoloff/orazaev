#include <message_processor.h>

void TGetMessageHandler::Process(const TTaskMessage& msg) {
    TStringHolder url = msg.GetUrl();
    printf("[%d] GET: %s, task_queue.size = %d\n", env_.thread_number, url->c_str(), env_.taskQueue->Size());

    TStringHolder html = std::make_shared<std::string>(env_.downloader->GetUrl(*url));
    TTaskMessage parseTask(url, html, T_PARSE, msg.GetDepth());
    env_.logQueue->Put(parseTask);
}

void TParseMessageHandler::Process(const TTaskMessage& msg) {
    if (msg.GetDepth() + 1 > env_.maxDownloadDepth) {
        TLogMessageHandler(env_).Process(msg);
        return;
    }
    std::set<std::string> links = TLinkParser::ParseText(msg);

    for (auto link : links) {
        if (env_.downloadedUrls->Insert(link)) {
            printf("[%d] Link(%d): '%s'\n", env_.thread_number, msg.GetDepth() + 1, link.c_str());
            TTaskMessage getTask(link, T_GET, msg.GetDepth() + 1);
            if (getTask.GetDepth() <= env_.maxDownloadDepth) {
                env_.logQueue->Put(getTask);
            }
        }
    }

    TLogMessageHandler(env_).Process(msg);
}

void TPoisonMessageHandler::Process(const TTaskMessage&) {
    printf("[%d] POISON:\n", env_.thread_number);

    TTaskMessage poisonedTask(T_POISON);
    env_.taskQueue->Clear();
    env_.taskQueue->Put(poisonedTask);
    env_.alive = false;
}

void TLogMessageHandler::Process(const TTaskMessage& msg) {
    printf("[%d] LOG: %s\n", env_.thread_number, msg.GetUrl()->c_str());
    TLogHolder log = env_.log;

    *log << "URL: " << *msg.GetUrl() << "\n";
    *log << "DEPTH: " << msg.GetDepth() << "\n";
    *log << "DUMP:\n\n";
    *log << *msg.GetHtml();
    *log << "\n\n\n";
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

