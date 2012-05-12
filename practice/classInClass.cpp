#include <iostream>

class A {
    int x;

    class B {
    public:
        B() {};
        void print_x() { std::cout << x; }
    };
    B b;

    A() {}
public:
    A(int a) : b(), x(a) {};

    void go() { b.print_x(); }
};

int main() {
    A a(20);
    a.go();
}
