#include <worker.h>

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
