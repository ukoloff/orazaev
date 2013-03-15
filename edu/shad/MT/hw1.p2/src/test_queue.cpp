#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    TWorkerEnvironment env("pages.dump");
    TWorkerEnvironment thread_parser_env(
        env.logQueue,
        env.taskQueue,
        env.downloadedUrls,
        env.log);

    std::string url = TUrlProcess::NormalizeUrl("ndev.vsv.lokos.net");
    url = TUrlProcess::NormalizeUrl("http://silikatsemey.kz/");
    env.taskQueue->Put(TTaskMessage(url, T_GET));
    env.downloadedUrls->Insert(url);

    // url = NormalizeUrl("http://www.youtube.com/user/ndevschool");
    // env.taskQueue->Put(TTaskMessage(url, T_GET));
    // env.downloadedUrls->Insert(url);

    // url = NormalizeUrl("ndev.vsv.lokos.net/pnpo.htm");
    // env.taskQueue->Put(TTaskMessage(url, T_GET));
    // env.downloadedUrls->Insert(url);
    //env.taskQueue->Put(TTaskMessage("http://google.ru", T_GET));
    //env.taskQueue->Put(TTaskMessage("http://yahoo.ru", T_GET));

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

    // May do it in main thread.
    thread_parser_env.thread_number = ++env.thread_number;
    TThreadGuard thread_parser(std::thread(StartWorker, thread_parser_env));


    std::this_thread::sleep_for(std::chrono::seconds(15));

    env.taskQueue->Clear();
    env.taskQueue->Put(TTaskMessage("", T_POISON));
    env.logQueue->Clear();
    env.logQueue->Put(TTaskMessage("", T_POISON));

    return 0;
}
