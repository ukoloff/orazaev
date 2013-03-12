#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

class THelloClass {
public:
    void operator()(int x) const {
        printf("[%d] Hello, concurent world!\n", x);
    }
};

void SomeFunction(int& data) {
    data += 100;
}

class TThreadGuard {
public:
    explicit TThreadGuard(std::thread stored)
            : stored(std::move(stored)) { }

    void detach() {
        stored.detach();
    }

    void join() {
        stored.join();
    }

    ~TThreadGuard() {
        if (stored.joinable()) {
            stored.join();
        }
    }

    TThreadGuard(const TThreadGuard&) = delete;
    TThreadGuard& operator=(const TThreadGuard&) = delete;

private:
    std::thread stored;
};

class TTestStdRefCopying {
public:
    TTestStdRefCopying() { }

    TTestStdRefCopying(const TTestStdRefCopying&) = delete;
    TTestStdRefCopying& operator=(const TTestStdRefCopying&) = delete;
};

void SomeTestFunc(TTestStdRefCopying& ref) {
    return;
}

int main() {
    TThreadGuard tGuard(std::thread(THelloClass(), 1));
    TThreadGuard kGuard(std::thread(THelloClass(), 2));
    TThreadGuard fooGuard(std::thread([] {
        std::cout << "Hello, concurent world!\n"; }));

    int data = 10;
    {
        TThreadGuard barGuard(std::thread(SomeFunction, data));
    }
    printf("data = %d\n", data);
    // Here will be 10.
    // That effect becouse constructor of std::thread ALWAYS copy his elements.
    // to fix that you should use std::ref wrapper.
    {
        TThreadGuard barGuard(std::thread(SomeFunction, std::ref(data)));
    }
    printf("data = %d\n", data);

    {
        TTestStdRefCopying ref;
        TThreadGuard barGuard(std::thread(SomeTestFunc, std::ref(ref)));
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread([](int x) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            printf("[%d] on air!\n", x);
        }, i));
    }

    std::for_each(threads.begin(), threads.end(),
            std::mem_fn(&std::thread::join));

    return 0;
}
