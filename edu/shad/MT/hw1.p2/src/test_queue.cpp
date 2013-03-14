#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    TWorkerEnvironment env;

    env.taskQueue->Put(TTaskMessage("http://ya.ru", T_GET));
    //env.taskQueue->Put(TTaskMessage("http://google.ru", T_GET));
    //env.taskQueue->Put(TTaskMessage("http://yahoo.ru", T_GET));

    std::cout << env.taskQueue->Size() << std::endl;

    TThreadGuard first(std::thread(StartWorker, env));
    TThreadGuard second(std::thread(StartWorker, env));
    TThreadGuard third(std::thread(StartWorker, env));
    TThreadGuard guard(std::thread(StartWorker, env));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    env.taskQueue->Put(TTaskMessage("", T_POISON));

    return 0;
}
