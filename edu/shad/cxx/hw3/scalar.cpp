#include <iostream>
#include <cassert>

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
struct scalar_product {
    template <typename T>
    inline T operator () (T* a, T* b) {
        return a[N - 1] * b[N - 1] + scalar_product<N - 1>()(a, b);
    }
};

template <>
struct scalar_product<1> {
    template <typename T>
    inline T operator () (T* a, T* b) {
        return a[0] * b[0];
    }
};

template <typename T>
void Dummy(T x);


int main() {
    int * const a = new int[3];
    int * const b = new int[3];
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    b[0] = 2;
    b[1] = 1;
    b[2] = 0;

    Dummy(scalar_product<3>()(a, b));
    

    delete [] a;
    delete [] b;
}
