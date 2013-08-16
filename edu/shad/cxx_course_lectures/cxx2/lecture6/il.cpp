#include <iostream>

struct A {
    int a;
    double b;
};

struct B {
    int a;
    double b;
};

void fun(A &&a) {
    printf("A!\n");
}

void fun(B &&b) {
    printf("A!\n");
}

int main() {
    fun({1, 1.38});
    return 0;
}
