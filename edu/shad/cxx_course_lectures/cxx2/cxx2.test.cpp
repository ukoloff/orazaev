#include <iostream>

class Foo {
public:
    void f(int) {
        std::cout << "Foo:f(int)" << std::endl;
    }

    void f(double) {
        std::cout << "Foo:f(double)" << std::endl;
    }
};

class Bar : public Foo {
public:
    void f(double) {
        std::cout << "Bar:f(double)" << std::endl;
    }
};

int main() {
    Bar b;
    b.f(2.0);
    b.f(2);

    return 0;
}
