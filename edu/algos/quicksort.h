/**
 * Quicksort template realization.
 *
 * See: http://en.wikipedia.org/wiki/Quicksort
 */
#pragma once

#include <algorithm>
#include <cstdlib>



/**
 * Partition step.
 */
template <typename T>
T partition(T begin, T end, T pivot) {
    std::swap(*(end - 1), *pivot);
    T storage = begin;

    for (T i = begin; i < (end - 1); ++i) {
        if (*i <= *(end - 1)) {
            std::swap(*storage, *i);
            ++storage;
        }
    }

    std::swap(*storage, *(end - 1));
    return storage;
}


/**
 * Quicksort.
 */
template <typename T>
void quicksort(T begin, T end) {
    if ((end - begin) <= 1)
        return;
    unsigned pivotIndex = std::rand() % (end - begin);
    T pivot = partition(begin, end, begin + pivotIndex);
    quicksort(begin, pivot);
    quicksort(++pivot, end);
}
