#include "crawler.h"

#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>

void TCrawler::Start() {
    std::string fullDumpFileName = _config.pathToDumpFile + "/" +
                                   _config.dumpFileName;
    TWorkerEnvironment thread_downloader_env(fullDumpFileName,
                                             _config.maxDepth);
    TWorkerEnvironment thread_parser_env(
        thread_downloader_env.resultQueue,
        thread_downloader_env.taskQueue,
        NULL,
        thread_downloader_env.downloadedUrls,
        thread_downloader_env.log,
        _config.maxDepth
    );

    std::string url = TUrlProcess::NormalizeUrl(_config.startUrl);
    thread_downloader_env.taskQueue->Put(TTaskMessage(url, T_GET, 0));
    thread_downloader_env.downloadedUrls->Insert(url);

    std::vector<std::thread> threads;

    // Start threads-downloaders.
    for (size_t i = 0; i < _config.numberOfDownloaderThreads; ++i) {
        threads.push_back(std::thread(StartWorker, thread_downloader_env));
        ++thread_downloader_env.thread_number;
    }

    // Start threads-parsers.
    thread_parser_env.thread_number = thread_downloader_env.thread_number;
    for (size_t i = 0; i < _config.numberOfParserThreads; ++i) {
        threads.push_back(std::thread(StartWorker, thread_parser_env));
        ++thread_parser_env.thread_number;
    }

    TWorkerEnvironment logger_env(
        thread_downloader_env.logQueue,
        thread_downloader_env.taskQueue,
        thread_downloader_env.resultQueue,
        thread_downloader_env.downloadedUrls,
        thread_downloader_env.log,
        _config.maxDepth
    );

    logger_env.thread_number = thread_parser_env.thread_number;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    StartWorker(logger_env);

    thread_downloader_env.taskQueue->Put(TTaskMessage(T_POISON));
    thread_parser_env.taskQueue->Put(TTaskMessage(T_POISON));

    for (auto thread = threads.begin(); thread != threads.end(); ++thread) {
        thread->join();
    }
}

void TSimpleCrawler::Start() {
    std::string fullDumpFileName = _config.pathToDumpFile + "/" +
                                   _config.dumpFileName;

    TMsgQueueHolder commonQueue(new TSimpleQueue<TTaskMessage>());

    TWorkerEnvironment env(
        commonQueue,
        commonQueue,
        commonQueue,
        TStringSetHolder(new TSynchronizedSet<std::string>()),
        TLogHolder(new TOfstreamGuard(fullDumpFileName)),
        _config.maxDepth
    );

    std::string url = TUrlProcess::NormalizeUrl(_config.startUrl);
    env.taskQueue->Put(TTaskMessage(url, T_GET, 0));
    env.downloadedUrls->Insert(url);

    StartWorker(env);
}
