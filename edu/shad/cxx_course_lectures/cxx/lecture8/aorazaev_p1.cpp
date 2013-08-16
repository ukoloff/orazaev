#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <iostream>
#include <list>


template <typename Iter, typename Category>
void bd_sort_impl(Iter begin, Iter end, Category) {
    std::vector<typename std::iterator_traits<Iter>::value_type>
            tempVector(begin, end);

    std::sort(tempVector.begin(), tempVector.end());

    std::copy(tempVector.begin(), tempVector.end(), begin);
}

template <typename Iter>
void bd_sort_impl(Iter begin, Iter end, std::random_access_iterator_tag) {
    std::sort(begin, end);
}

template <typename Iter>
void bd_sort(Iter begin, Iter end) {
    bd_sort_impl(begin, end
            , typename std::iterator_traits<Iter>::iterator_category());
}

template <typename T>
void printElements(T begin, T end, std::string space = " ") {
    for (T it = begin; it != end; ++it) {
        std::cout << *it << space;
    }
    std::cout << std::endl;
}


int main() {
    int a[9] = {
        4, 3, 6, 1, 9, 10, 32, 1, 0
    };

    std::list<int> li(a, a +9);

    bd_sort(a, a + 9);
    printElements(a, a + 9);


    bd_sort(li.begin(), li.end());
    printElements(li.begin(), li.end());

    return 0;
}
