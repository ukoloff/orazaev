#include <queue.h>
#include <thread_guard.h>
#include <thread>
#include <memory>
#include <iostream>
#include <messages.h>


/**
    @brief run functor for std::thread.
    Process messages until get poisoned message.
*/
class TThreadProcess {
public:
    TThreadProcess(
            const std::shared_ptr<TSynchronizedQueue<TTaskMessage> >& messages)
    : messages(messages) { }

    void operator()() {
        bool alive = true;
        while (alive) {
            TTaskMessage message = messages->Take();
            switch (message.GetType()) {
                case T_GET:
                    ProcessGetMessage(message);
                    break;
                case T_PARSE:
                    ProcessParseMessage(message);
                    break;
                case T_POISON:
                    alive = false;
                    ProcessPoisonedMessage();
                    break;
                case T_LOG:
                    ProcessLogMessage(message);
                    break;
            };
        }
    }

    void ProcessGetMessage(const TTaskMessage& message) {
        printf("GET: %s\n", message.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin get message = '%s'", message.GetData()->c_str());
        TTaskMessage logTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_LOG);
        messages->Put(logTask);

        sprintf(buffer, "Parsing message = '%s'", message.GetData()->c_str());
        TTaskMessage parseTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_PARSE);
        messages->Put(parseTask);
    }

    void ProcessParseMessage(const TTaskMessage& message) {
        printf("PARSE: %s\n", message.GetData()->c_str());
        char buffer[500];

        sprintf(buffer, "Loggin parse message = '%s'", message.GetData()->c_str());
        TTaskMessage logTask(
            std::shared_ptr<std::string>(new std::string(buffer)),
            T_LOG);
        messages->Put(logTask);
    }

    void ProcessPoisonedMessage() {
        printf("POISON:\n");

        TTaskMessage poisonedTask(
            std::shared_ptr<std::string>(),
            T_POISON);
        messages->Put(poisonedTask);
    }

    void ProcessLogMessage(const TTaskMessage& message) {
        printf("LOG: %s\n", message.GetData()->c_str());
    }


private:
    std::shared_ptr<TSynchronizedQueue<TTaskMessage> > messages;
};

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
        TThreadProcess a(messages);
        a();
    }));
    TThreadGuard second(std::thread([&messages] {
        TThreadProcess a(messages);
        a();
    }));
    TThreadGuard third(std::thread([&messages] {
        TThreadProcess a(messages);
        a();
    }));

    TThreadProcess p(messages);
    TThreadGuard guard(std::thread(std::ref(p)));

    std::this_thread::sleep_for(std::chrono::seconds(3));

    messages->Put(TTaskMessage(
        std::shared_ptr<std::string>(),
        T_POISON
    ));

    return 0;
}
