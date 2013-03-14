#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

/**
    class TSynchronizedQueue.
    @brief synchonized queue realization from lecture.
*/
template <typename T>
class TSynchronizedQueue {
public:
    TSynchronizedQueue() { }

    T Take();
    void Put(const T&);
    /** Not consistant Size() */
    size_t Size() const;

    /** You should to Put poisoned task after call Clear(). */
    void Clear();

private:
    TSynchronizedQueue(const TSynchronizedQueue&) = delete;
    TSynchronizedQueue& operator=(const TSynchronizedQueue&) = delete;

private:
    std::queue<T> queue_;
    std::condition_variable cond_;
    std::mutex mutex_;
};



template <typename T>
T TSynchronizedQueue<T>::Take() {
    std::unique_lock<std::mutex> ulock(mutex_);

    while (queue_.empty()) {
        cond_.wait(ulock);
    }

    T val = queue_.front();
    queue_.pop();
    return val;
}

template <typename T>
void TSynchronizedQueue<T>::Put(const T& elem) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(elem);
    cond_.notify_one();
}

template <typename T>
void TSynchronizedQueue<T>::Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_ = std::queue<T>();
}

template <typename T>
size_t TSynchronizedQueue<T>::Size() const {
    return queue_.size();
}
