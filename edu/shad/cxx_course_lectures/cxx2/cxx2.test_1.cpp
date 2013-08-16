#include <iostream>

int main() {
    char* a = "abcd";
    char b[] = "abcd";
    char d[30];

    std::cout << sizeof a << " " << sizeof b << " " << sizeof d << std::endl;
}
