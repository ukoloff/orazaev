#include <iostream>

class A {
    int data;
    public:
    virtual void f() {
        std::cout << "A::f()" << std::endl;
    }
};

class B {
    int data;
    public:
    void f() {
        std::cout << "B::f()" << std::endl;
    }
};

int main() {
    A a;
    B b;
    std::cout << "sizeof(A()) = " << sizeof(a) << std::endl;
    std::cout << "sizeof(B()) = " << sizeof(b) << std::endl;
    std::cout << "---------------------------------------\n"
              << "A with virtual function, B without." << std::endl;
}
