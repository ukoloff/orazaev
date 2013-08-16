#include<iostream>
#include<vector>

template <typename T>
struct C {
    std::vector<T> v;
};

int main() {
    C<double> c;
    return 0;
}
