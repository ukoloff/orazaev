#pragma once

#define BOOST_TEST_DYN_LINK

#include <boost/test/included/unit_test.hpp>
#include <vector>


template <typename T>
bool isSorted(T begin, T end) {
    for (T i = begin; i < (end - 1); ++i) {
        if (*i > *(i + 1)) {
            return false;
        }
    }
    return true;
}

std::vector<int> randomVector(size_t maxLen = 50, int maxVal = 1000) {
    size_t len = rand() % maxLen + 1;
    std::vector<int> vec(len, 0);
    for (int i = 0; i < len; ++i) {
        vec[i] = rand() % maxVal;
    }

    return vec;
}
