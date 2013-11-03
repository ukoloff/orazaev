#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE quicksort

#include "quicksort.h"
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

BOOST_AUTO_TEST_CASE(sorting) {
    std::vector<int> vec {5, 4, 3, 1, 1, 6, 7, 9, 12, 7, 24};
    quicksort(vec.begin(), vec.end());
    BOOST_CHECK(isSorted(vec.begin(), vec.end()));
}

BOOST_AUTO_TEST_CASE(stress_test) {
    for (int i = 0; i < 10000; ++i) {
        std::vector<int> vec(randomVector());
        quicksort(vec.begin(), vec.end());
        BOOST_CHECK(isSorted(vec.begin(), vec.end()));
    }
}
