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

    void Clear();

private:
    TSynchronizedQueue(const TSynchronizedQueue&) = delete;
    TSynchronizedQueue& operator=(const TSynchronizedQueue&) = delete;

private:
    std::queue<T> queue_;
    std::condition_variable cond_;
    std::mutex mutex_;
};


/**
    class TSynchronizedCirledBuffer.
    @brief Circled buffer realization.
*/
template <typename T, size_t BUF_SIZE>
class TSynchronizedCirledBuffer {
public:
    TSynchronizedCirledBuffer()
        : start(1)
    { }

    T Take();
    void Put(const T&);

    /** Non consistant Size(), IsEmpty() and IsFull() */
    inline bool Size() const { return (start + BUF_SIZE - end) % BUF_SIZE - 1; }
    inline bool IsEmpty() const { return (end + 1) % BUF_SIZE == start; }
    inline bool IsFull() const { return (start + 1) % BUF_SIZE == end; }

    void Clear();
private:
    size_t start;
    size_t end;

    T buffer_[BUF_SIZE];
    std::condition_variable cond_empty;
    std::condition_variable cond_full;
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

template <typename T, size_t BS>
T TSynchronizedCirledBuffer<T, BS>::Take() {
    // FIXME(orazaev@): fill
    std::unique_lock<std::mutex> ulock(mutex_);

    while (IsEmpty()) {
        cond_empty.wait(ulock);
    }

    T result = buffer_[++end];
    cond_full.notify_one();

    return result;
}

template <typename T, size_t BS>
void TSynchronizedCirledBuffer<T, BS>::Put(const T& elem) {
    // FIXME(orazaev@): fill
    std::unique_lock<std::mutex> ulock(mutex_);

    while (IsFull()) {
        cond_full.wait(ulock);
    }

    buffer_[start++] = elem;
    cond_empty.notify_one();
}

template <typename T, size_t BS>
void TSynchronizedCirledBuffer<T, BS>::Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    end = start;
}
