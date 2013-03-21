#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

template <typename T>
class TQueue {
protected:
    TQueue()  { }
    virtual ~TQueue() { }

public:
    virtual T Take() = 0;
    virtual void Put(const T&) = 0;

    virtual size_t Size() const = 0;
    virtual void Clear() = 0;
};

/**
    class TSynchronizedQueue.
    @brief Synchonized queue realization from lecture.
*/
template <typename T>
class TSynchronizedQueue : public TQueue<T> {
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
    class TSynchronizedCircledBuffer.
    @brief Circled buffer realization.
*/
template <typename T, size_t BUF_SIZE>
class TSynchronizedCircledBuffer : public TQueue<T> {
public:
    TSynchronizedCircledBuffer()
        : start(1)
    { }

    T Take();
    void Put(const T&);

    /** Non consistant Size(), IsEmpty() and IsFull() */
    inline size_t Size() const { return (start + BUF_SIZE - end) % BUF_SIZE - 1; }
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


/**
    class TSimpleQueue
    @brief Singlethreaded wrapper around std::queue.

    Need for singlethreaded crawler.
*/
template <typename T>
class TSimpleQueue : public TQueue<T> {
public:
    TSimpleQueue() { }

    virtual T Take() {
        T res = queue_.front();
        queue_.pop();
        return res;
    }
    virtual void Put(const T& elem) { queue_.push(elem); }

    virtual size_t Size() const { return queue_.size(); }
    virtual void Clear() { queue_ = std::queue<T>(); }

private:
    std::queue<T> queue_;
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
T TSynchronizedCircledBuffer<T, BS>::Take() {
    std::unique_lock<std::mutex> ulock(mutex_);

    while (IsEmpty()) {
        cond_empty.wait(ulock);
    }

    end = (end + 1) % BS;
    T result = buffer_[end];
    cond_full.notify_one();

    return result;
}

template <typename T, size_t BS>
void TSynchronizedCircledBuffer<T, BS>::Put(const T& elem) {
    std::unique_lock<std::mutex> ulock(mutex_);

    while (IsFull()) {
        cond_full.wait(ulock, [this](){ return this->Size() < BS / 2; });
    }

    new(buffer_ + start++) T(elem);
    start %= BS;
    cond_empty.notify_one();
}

template <typename T, size_t BS>
void TSynchronizedCircledBuffer<T, BS>::Clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    end = start;
}
