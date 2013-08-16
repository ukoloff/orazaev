#include<iostream>

template <typename T>
void printBinary(T number) {
    T x = 1;
    for(int shift = sizeof(T) * 8 - 1; shift >= 0; --shift) {
        std::cout << (bool)((number & (x << shift)) >> shift);
    }
    
    std::cout << std::endl;
}

int main() {
    char c = 4;
    int x = 7;
    unsigned u = 8;
    long l = 15;
    
    printBinary(c);
    printBinary(x);
    printBinary(u);
    printBinary(l);
    printBinary<int>(-1);

    return 0;
}
