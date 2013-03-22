/**
    @brief Unordered set thread safe version.

    @author Aman Orazaev
*/
#pragma once
#include <unordered_set>
#include <mutex>

/**
    @brief Abstract interface for crawler set.
*/
template <typename T>
class TSet {
public:
    virtual bool Insert(const T& elem) = 0;
    virtual size_t Size() const = 0;

    virtual ~TSet() { }
};

/**
    @brief Lock-based thread-safe unordered set.
*/
template <typename T>
class TSynchronizedSet : public TSet<T> {
public:
    TSynchronizedSet(size_t maxPages)
        : maxPages_(maxPages)
    { }

    /**
        @brief try to insert element if it isn't in set.
        @return true - if element was added to set. false - if element was already in set.
    */
    virtual bool Insert(const T& elem);

    virtual size_t Size() const { return uset_.size(); }

private:
    size_t maxPages_;
    std::unordered_set<T> uset_;
    std::mutex mutex_;
};

/**
    @brief Singlethreaded std::unordered_set wrapper.
*/
template <typename T>
class TSimpleSet : public TSet<T> {
public:
    TSimpleSet(size_t maxPages)
        : maxPages_(maxPages)
    { }

    /**
        @brief try to insert element if it isn't in set.
        @return true - if element was added to set. false - if element was already in set.
    */
    virtual bool Insert(const T& elem);

    virtual size_t Size() const { return uset_.size(); }

private:
    size_t maxPages_;
    std::unordered_set<T> uset_;
};

template <typename T>
bool TSynchronizedSet<T>::Insert(const T& elem) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (uset_.count(elem)) {
        return false;
    }
    if (uset_.size() == maxPages_) {
        return false;
    }

    uset_.insert(elem);
    return true;
}

template <typename T>
bool TSimpleSet<T>::Insert(const T& elem) {
    if (uset_.count(elem)) {
        return false;
    }
    if (uset_.size() == maxPages_) {
        return false;
    }

    uset_.insert(elem);
    return true;
}
