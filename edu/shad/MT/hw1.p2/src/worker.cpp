#include <worker.h>

TThreadWorker::TThreadWorker(const TWorkerEnvironment& env)
    : env_(env)
{
    /// Create downloader for this thread.
    env_.downloader = std::make_shared<TPageDownloader>(TPageDownloader());
}

void TThreadWorker::Run() {
    while (env_.alive) {
        TTaskMessage message = env_.taskQueue->Take();
        TMsgProcessor::Process(message, env_);
    }

    printf("[%d] Total downloading time for this thread is %f sec.\n",
           env_.thread_number,
           env_.downloader->GetTotalTime());
}

void StartWorker(const TWorkerEnvironment& env) {
    TThreadWorker worker(env);
    worker.Run();
}
