#define BOOST_TEST_MODULE quicksort

#include "quicksort.h"
#include "common_ut.h"


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
