#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    // FIXME(orazaev@): calculate download depth.
    size_t maxDownloadDepth = 2;

    TWorkerEnvironment env(
        "pages.dump",
        maxDownloadDepth);
    TWorkerEnvironment thread_parser_env(
        env.resultQueue,
        env.taskQueue,
        /* Do not create dump messages. */ NULL,
        env.downloadedUrls,
        env.log,
        maxDownloadDepth);

    std::string url = TUrlProcess::NormalizeUrl("ndev.vsv.lokos.net");
    url = TUrlProcess::NormalizeUrl("http://silikatsemey.kz/");
    env.taskQueue->Put(TTaskMessage(url, T_GET, 0));
    env.downloadedUrls->Insert(url);

    std::cout << env.taskQueue->Size() << std::endl;

    TThreadGuard first(std::thread(StartWorker, env));
    ++env.thread_number;
    TThreadGuard second(std::thread(StartWorker, env));
    ++env.thread_number;
    TThreadGuard third(std::thread(StartWorker, env));
    ++env.thread_number;
    TThreadGuard fourth(std::thread(StartWorker, env));
    // ++env.thread_number;
    // TThreadGuard five(std::thread(StartWorker, env));

    // Start thread-parser.
    thread_parser_env.thread_number = ++env.thread_number;
    TThreadGuard thread_parser(std::thread(StartWorker, thread_parser_env));

    TWorkerEnvironment thread_logger_env(
        env.logQueue,
        env.taskQueue,
        env.resultQueue,
        env.downloadedUrls,
        env.log,
        maxDownloadDepth);

    thread_logger_env.thread_number = ++env.thread_number;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    StartWorker(thread_logger_env);

    // env.taskQueue->Clear();
    // env.resultQueue->Clear();
    env.taskQueue->Put(TTaskMessage(T_POISON));
    env.resultQueue->Put(TTaskMessage(T_POISON));

    return 0;
}
