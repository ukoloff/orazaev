#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    TWorkerEnvironment env;

    std::string url = NormalizeUrl("ndev.vsv.lokos.net");
    url = NormalizeUrl("http://silikatsemey.kz/");
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
    env.thread_number++;
    TThreadGuard second(std::thread(StartWorker, env));
    //env.thread_number++;
    //TThreadGuard third(std::thread(StartWorker, env));
    //TThreadGuard guard(std::thread(StartWorker, env));

    std::this_thread::sleep_for(std::chrono::seconds(15));

    env.taskQueue->Clear();
    env.taskQueue->Put(TTaskMessage("", T_POISON));

    return 0;
}
