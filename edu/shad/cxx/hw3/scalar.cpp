#include <iostream>
#include <cassert>
#include <time.h>
#include <cstdlib>
#include <unistd.h>

// template <typename T, T* A, T* B, size_t N>
// struct metaScalar {
//     static T result = A[N - 1] * B[N - 1] + metaScalar<T, A, B, N - 1>::result;
// };
//
// template <typename T, T* A, T* B>
// struct metaScalar<T, A, B, 1> {
//     static T result = A[0] * B[0];
// };

template <size_t N>
struct TScalar_product {
    template <typename T>
    inline T operator () (T* first, T* second) {
        return first[N - 1] * second[N - 1] +
            TScalar_product<N - 1>()(first, second);
    }
};

template <>
struct TScalar_product<1> {
    template <typename T>
    inline T operator () (T* first, T* second) {
        return first[0] * second[0];
    }
};



static const size_t TESTSIZE = 100000;

int main() {
    int foo[TESTSIZE] = {1};
    int bar[TESTSIZE] = {1};

    TScalar_product<TESTSIZE> scalar_product;

    clock_t start;
    clock_t end;

    start = clock();
    std::cout << scalar_product(foo, bar) << std::endl;
    end = clock();

    double time = static_cast<double>(end - start);

    std::cout << "template scalar_product speed: "
              << time
              << std::endl;

    int cycle_scalar_product = 0;

    start = clock();
    for (size_t i = 0; i < TESTSIZE; ++i) {
        cycle_scalar_product += foo[i] * bar[i];
    }
    end = clock();

    std::cout << cycle_scalar_product << std::endl;
    std::cout << "cycle scalar_product speed: "
              << static_cast<double>(end - start)
              << std::endl;

    return 0;
}
