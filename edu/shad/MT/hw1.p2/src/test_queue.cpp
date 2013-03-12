#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    TWorkerEnvironment env;

    env.taskQueue->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://ya.ru")),
        T_GET
    ));
    env.taskQueue->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://google.ru")),
        T_GET
    ));
    env.taskQueue->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://yahoo.ru")),
        T_GET
    ));

    std::cout << env.taskQueue->Size() << std::endl;

    TThreadGuard first(std::thread([&env] {
        TThreadWorker a(env);
        a();
    }));
    TThreadGuard second(std::thread([&env] {
        TThreadWorker a(env);
        a();
    }));
    TThreadGuard third(std::thread([&env] {
        TThreadWorker a(env);
        a();
    }));

    TThreadWorker p(env);
    TThreadGuard guard(std::thread(std::ref(p)));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    env.taskQueue->Put(TTaskMessage(
        std::shared_ptr<std::string>(),
        T_POISON
    ));

    return 0;
}
