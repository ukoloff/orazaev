#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

template <typename Iter>
Iter unique(Iter first, Iter last) {
    if (first == last)
        return first;

    Iter lastUniq = first;

    while (first != last) {
        if (*first != *lastUniq) {
            ++lastUniq;
            *lastUniq = *first;
        }
        ++first;
    }

    return ++lastUniq;
}

int main() {

    std::vector<int> v;
    std::copy(
        std::istream_iterator<int>(std::cin),
        std::istream_iterator<int>(),
        std::back_inserter(v)
    );

    std::vector<int>::iterator uend = ::unique(v.begin(), v.end());
    for (std::vector<int>::const_iterator it = v.begin();
         it != uend; ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
