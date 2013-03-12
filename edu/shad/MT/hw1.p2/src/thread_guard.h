#pragma once

#include <thread>

/**
    class TThreadGuard.
    @brief RAII ideome for std::thread.
*/
class TThreadGuard {
public:
    TThreadGuard(std::thread t)
        : stored_(std::move(t)) { }

    inline void Join() { stored_.join(); }

    inline void Detach() { stored_.detach(); }

    ~TThreadGuard() {
        if (stored_.joinable()) {
            stored_.join();
        }
    }

private:
    TThreadGuard(const TThreadGuard&) = delete;
    TThreadGuard& operator=(const TThreadGuard&) = delete;
private:
    std::thread stored_;
};
