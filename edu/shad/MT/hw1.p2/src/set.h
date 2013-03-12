/**
    @brief Unordered set thread safe version.
    
    @author Aman Orazaev
*/
#pragma once
#include <unordered_set>
#include <mutex>

template <typename T>
class TSynchronizedSet {
public:
    TSynchronizedSet() { }

    /** 
        @brief try to insert element if it isn't in set.
        This is just one public method for work with set.

        @return true - if element was added to set. false - if element was already in set.
    */
    bool Insert(const T& elem);

private:
    std::unordered_set<T> uset_;
    std::mutex mutex_;
};

template <typename T>
bool TSynchronizedSet<T>::Insert(const T& elem) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (uset_.count(elem)) {
        return false;
    }

    uset_.insert(elem);
    return true;
}
