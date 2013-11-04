#define BOOST_TEST_MODULE mergeSort

#include "mergesort.h"
#include "common_ut.h"


BOOST_AUTO_TEST_CASE(sorting) {
    std::vector<int> vec {5, 4, 3, 1, 1, 6, 7, 9, 12, 7, 24};
    mergeSort(vec.begin(), vec.end());
    BOOST_CHECK(isSorted(vec.begin(), vec.end()));
}

BOOST_AUTO_TEST_CASE(sorting_ptr) {
    int vec[] = {5, 4, 3, 1, 1, 6, 7, 9, 12, 7, 24};
    size_t vecSize = sizeof(vec) / sizeof(int);
    mergeSort(vec, vec + vecSize);
    BOOST_CHECK(isSorted(vec, vec + vecSize));
}

BOOST_AUTO_TEST_CASE(stress_test) {
    for (int i = 0; i < 10000; ++i) {
        std::vector<int> vec(randomVector());
        mergeSort(vec.begin(), vec.end());
        BOOST_CHECK(isSorted(vec.begin(), vec.end()));
    }
}
