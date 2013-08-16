#include<iostream>

bool getBit(int data, int pos) {
    return (data & (1 << (pos - 1))) >> (pos - 1);
    //return data >> (pos - 1) & 1
}

void setBit(int * const data, int pos, int value) {
    *data |= 1 << (pos - 1)
    if (value == 0)
        *data ^= 1 << (pos - 1)
}

int main() {
    int x;
    int pos;
    std::cin >> x;
    std::cin >> pos;
    std::cout << getBit(x, pos) << std::endl;
}
