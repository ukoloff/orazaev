#include <messages.h>

void TGetMessageHandler::Process(const TTaskMessage& msg) {
    TStringHolder url = msg.GetUrl();
    printf("GET: %s\n", url->c_str());

    TStringHolder html = std::make_shared<std::string>(env_.downloader->GetUrl(*url));
    TTaskMessage parseTask(url, html, T_PARSE);
    env_.taskQueue->Put(parseTask);

    TTaskMessage logTask(url, html, T_LOG);
    env_.logQueue->Put(logTask);
}

void TParseMessageHandler::Process(const TTaskMessage& msg) {
    std::set<std::string> links = TLinkParser::ParseText(*msg.GetHtml());

    for (auto link : links) {
        printf("Link: %s\n", link.c_str());
    }
}

void TPoisonMessageHandler::Process(const TTaskMessage&) {
    printf("POISON:\n");

    TTaskMessage poisonedTask(T_POISON);
    env_.taskQueue->Put(poisonedTask);
    env_.alive = false;
}

void TLogMessageHandler::Process(const TTaskMessage& msg) {
    printf("LOG: %s\n", msg.GetUrl()->c_str());
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

