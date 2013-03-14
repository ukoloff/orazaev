#include <messages.h>

void TGetMessageHandler::Process(const TTaskMessage& msg) {
    printf("GET: %s\n", msg.GetData()->c_str());

    TTaskMessage parseTask(env_.downloader->GetUrl(*msg.GetData()), T_PARSE);
    env_.taskQueue->Put(parseTask);
}

void TParseMessageHandler::Process(const TTaskMessage& msg) {
    /// printf("PARSE:\n%s\n\n\n", msg.GetData()->c_str());
    std::set<std::string> links = TLinkParser::ParseText(*msg.GetData());

    for (auto link : links) {
        printf("Link: %s\n", link.c_str());
    }
}

void TPoisonMessageHandler::Process(const TTaskMessage&) {
    printf("POISON:\n");

    TTaskMessage poisonedTask("", T_POISON);
    env_.taskQueue->Put(poisonedTask);
    env_.alive = false;
}

void TLogMessageHandler::Process(const TTaskMessage& msg) {
    printf("LOG: %s\n", msg.GetData()->c_str());
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

