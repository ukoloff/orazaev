#include <worker.h>

void TGetMessageHandler::Process(const TTaskMessage& msg) {
    printf("GET: %s\n", msg.GetData()->c_str());
    char buffer[500];

    sprintf(buffer, "Loggin get message = '%s'", msg.GetData()->c_str());
    TTaskMessage logTask(buffer, T_LOG);
    env_.taskQueue->Put(logTask);

    sprintf(buffer, "Parsing message = '%s'", msg.GetData()->c_str());
    TTaskMessage parseTask(buffer, T_PARSE);
    env_.taskQueue->Put(parseTask);
}

void TParseMessageHandler::Process(const TTaskMessage& msg) {
    printf("PARSE: %s\n", msg.GetData()->c_str());
    char buffer[500];

    sprintf(buffer, "Loggin parse message = '%s'", msg.GetData()->c_str());
    TTaskMessage logTask(buffer, T_LOG);
    env_.taskQueue->Put(logTask);
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

void TThreadWorker::Run() {
    while (env_.alive) {
        TTaskMessage message = env_.taskQueue->Take();
        TMsgProcessor::Process(message, env_);
    }
}

void StartWorker(const TWorkerEnvironment& env) {
    TThreadWorker worker(env);
    worker.Run();
}
