#include<iostream>

int main() {
    int z = 1;
    ++++z--;
    std::cout << (++z--) << std::endl;
    return 0;
}
