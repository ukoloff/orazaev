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

template <bool TRUE>
struct static_assert {
};

template <>
struct static_assert<false>;

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

template <typename T>
void Dummy(T x);



static const size_t TESTSIZE = 1000;

int main() {
    int foo[TESTSIZE] = {1};
    int bar[TESTSIZE] = {1};

    TScalar_product<TESTSIZE> scalar_product;

    clock_t start;
    clock_t end;

    start = clock();
    std::cout << start << std::endl;
    std::cout << scalar_product(foo, bar) << std::endl;
    for (int i = 0; i >= 0; ++i) {

    }
    end = clock();
    std::cout << end << std::endl;

    double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;

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
