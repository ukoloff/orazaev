#include<stdio.h>
#include<iostream>

template <int N, int Div>
struct Is {
    static const bool Prime = N % Div && Is<N, Div - 1>::Prime;
};

template <int N>
struct Is<N, 1> {
    static const bool Prime = true;
};

template <int N>
struct IsPrime {
    static const bool ans = Is<N, N - 1>::Prime;
};


template <int N, bool T>
struct PrintIfPrime;

template <int N>
struct PrintIfPrime<N, false> {
    static const bool print = false;
};

template <int N>
struct PrintPrime {
    static const bool print = PrintIfPrime<N, IsPrime<N>::ans>::print
            && PrintPrime<N - 1>::print;
};

template <>
struct PrintPrime<1> {
    static const bool print = false;
};

int main() {
    std::cout << IsPrime<11>::ans << std::endl;

    PrintPrime<10>::print;

    return 0;
}
