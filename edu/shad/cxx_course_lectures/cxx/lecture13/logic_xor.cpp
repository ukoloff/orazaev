#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>

bool logical_xor(bool a, bool b) {
    return !(a == true && b == true) && !(a == false && b == false);
}

int main() {
    bool a[] = {
        1, 0, 1, 0, 1, 0, 1, 0, 1, 0
    };


    bool b[10];

    std::transform(
        a,
        a + 10,
        std::ostream_iterator<bool>(std::cout, " "),
        std::bind2nd(std::ptr_fun(logical_xor), 1)
    );


    return 0;
}
