/**
 * Merge sort template realization.
 *
 * See: http://en.wikipedia.org/wiki/Merge_sort
 */
#pragma once

#include <boost/scoped_array.hpp>

template <typename T, typename C>
void mergeSort_(T begin, T end, C tmp) {
    if (end - begin < 2) {
        return;
    }

    T middle = begin + (end - begin)/2;
    mergeSort_(begin, middle, tmp);
    mergeSort_(middle, end, tmp);
    merge_(begin, middle, end, tmp);
}

template <typename T, typename C>
void merge_(T begin, T middle, T end, C tmp) {
    T i = begin;
    T j = middle;
    C k = tmp;
    while (i < middle || j < end) {
        if (i < middle && (j >= end || *i < *j)) {
            *k++ = *i++;
        } else {
            *k++ = *j++;
        }
    }
    for (k = tmp, i = begin; i < end; *i++ = *k++);
}

/**
 * Merge sort poniters template specialization
 */
template <typename T>
void mergeSort(T begin, T end) {
    typedef typename T::value_type value_type;
    boost::scoped_array<value_type> tmp(new value_type[end - begin]);
    mergeSort_(begin, end, tmp.get());
}

template <typename T>
void mergeSort(T* begin, T* end) {
    boost::scoped_array<T> tmp(new T[end - begin]);
    mergeSort_(begin, end, tmp.get());
}
