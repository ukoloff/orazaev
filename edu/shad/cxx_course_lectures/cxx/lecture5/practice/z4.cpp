#include<iostream>
#include<vector>


template <typename T, typename R, typename F>
R copy_if(T begin, T end, R dest, F P) {
    while(begin != end) {
        if (P(*begin)) {
            *dest++ = *begin++;
        } else {
            ++begin;
        }
    }

    return dest;
}

bool predicat(int x) {
    return x > 0;
}

int main() {

    int a[5] = {0, 1, 2, -3, 5};
    std::vector<int> v(5, 0);
    
    std::vector<int>::const_iterator vend = copy_if(a, a + 5, v.begin(), &predicat);

    for(std::vector<int>::const_iterator it = v.begin(); 
        it != vend; ++it)
        std::cout << *it << " ";

    std::cout << std::endl;
    return 0;
}

