#include <iostream>

int some_func(int x, int y) {
    return x + y;
}

int main() {
    std::cout << (int) some_func << std::endl;
    return 0;
}
