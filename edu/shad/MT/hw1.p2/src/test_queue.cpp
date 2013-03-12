#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>
#include <worker.h>



int main() {
    std::shared_ptr<TSynchronizedQueue<TTaskMessage> > messages(
            new TSynchronizedQueue<TTaskMessage>());

    messages->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://ya.ru")),
        T_GET
    ));
    messages->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://google.ru")),
        T_GET
    ));
    messages->Put(TTaskMessage(
        std::shared_ptr<std::string>(new std::string("http://yahoo.ru")),
        T_GET
    ));

    std::cout << messages->Size() << std::endl;

    TThreadGuard first(std::thread([&messages] {
        TThreadWorker a(messages);
        a();
    }));
    TThreadGuard second(std::thread([&messages] {
        TThreadWorker a(messages);
        a();
    }));
    TThreadGuard third(std::thread([&messages] {
        TThreadWorker a(messages);
        a();
    }));

    TThreadWorker p(messages);
    TThreadGuard guard(std::thread(std::ref(p)));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    messages->Put(TTaskMessage(
        std::shared_ptr<std::string>(),
        T_POISON
    ));

    return 0;
}
