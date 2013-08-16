#include <iostream>

int main() {
    const int a = 1;

    const_cast<int&>(a) = 2;

    std::cout << a << std::endl;
    return 0;
}
