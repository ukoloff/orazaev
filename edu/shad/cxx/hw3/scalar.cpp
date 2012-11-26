#include <time.h>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

template <size_t N>
class TFastScalarProduct;

template <size_t N>
struct TNativeScalarProduct;



template <size_t N, typename T>
inline T scalar_product(T* first, T* second) {
    return TFastScalarProduct<N>().product(first, second);
}




////////////////////////////// TNativeScalarProduct ///////////////////////////

template <size_t N>
struct TNativeScalarProduct {
    template <typename T>
    inline T operator () (T* first, T* second) {
        return first[N - 1] * second[N - 1] +
            TNativeScalarProduct<N - 1>()(first, second);
    }
};



template <>
struct TNativeScalarProduct<1> {
    template <typename T>
    inline T operator () (T* first, T* second) {
        return first[0] * second[0];
    }
};



////////////////////////////// TFastScalarProduct /////////////////////////////

template <size_t N>
class TFastScalarProduct {
    template <typename T>
    inline T prod10(T* a, T* b) const {
        return
        a[N - 1] * b[N - 1] + a[N - 2] * b[N - 2] +
        a[N - 3] * b[N - 3] + a[N - 4] * b[N - 4] +
        a[N - 5] * b[N - 5] + a[N - 6] * b[N - 6] +
        a[N - 7] * b[N - 7] + a[N - 8] * b[N - 8] +
        a[N - 9] * b[N - 9] + a[N - 10] * b[N - 10];
    }

    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return prod10(foo, bar) + TFastScalarProduct<N - 10>().product(foo, bar);
    }
};

template <>
class TFastScalarProduct<9> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4] + foo[5] * bar[5] +
        foo[6] * bar[6] + foo[7] * bar[7] +
        foo[8] * bar[8];
    }
};

template <>
class TFastScalarProduct<8> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4] + foo[5] * bar[5] +
        foo[6] * bar[6] + foo[7] * bar[7];
    }
};

template <>
class TFastScalarProduct<7> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4] + foo[5] * bar[5] +
        foo[6] * bar[6];
    }
};

template <>
class TFastScalarProduct<6> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4] + foo[5] * bar[5];
    }
};

template <>
class TFastScalarProduct<5> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4];
    }
};

template <>
class TFastScalarProduct<4> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3];
    }
};

template <>
class TFastScalarProduct<3> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2];
    }
};

template <>
class TFastScalarProduct<2> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return foo[0] * bar[0] + foo[1] * bar[1];
    }
};

template <>
class TFastScalarProduct<1> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return foo[0] * bar[0];
    }
};

template <>
class TFastScalarProduct<0> {
    public:
    template <typename T>
    inline T product(T* foo, T* bar) const {
        return 0;
    }
};



///////////////////////////////// TTimer //////////////////////////////////////

class TTimer {
    clock_t startClocks;
    clock_t endClocks;
    public:
    TTimer()
        : startClocks(0)
        , endClocks(0) {
    }

    void start() {
        startClocks = clock();
    }

    void stop() {
        endClocks = clock();
    }

    double getSeconds() const {
        return static_cast<double>(
            endClocks - startClocks
        ) / CLOCKS_PER_SEC;
    }

    clock_t getClocks() const {
        return endClocks - startClocks;
    }
};



static const size_t vectorSize = 20;
static const size_t numberOfStarts = 10000000;



int main() {
    TTimer timer;


    int foo[vectorSize];
    int bar[vectorSize];
    for (size_t i = 0; i < vectorSize; ++i) {
        foo[i] = bar[i] = 1;
    }




    timer.start();
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        scalar_product<vectorSize>(foo, bar);
    }
    timer.stop();

    std::cout << "function scalar_product<N> speed: "
              << timer.getSeconds() << std::endl;





    timer.start();
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        TFastScalarProduct<vectorSize>().product(foo, bar);
    }
    timer.stop();

    std::cout << "class TFastScalarProduct<N> speed: "
              << timer.getSeconds() << std::endl;





    timer.start();
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        TNativeScalarProduct<vectorSize>()(foo, bar);
    }
    timer.stop();

    std::cout << "struct TNativeScalarProduct<N> speed: "
              << timer.getSeconds() << std::endl;





    timer.start();
    int cycle_scalar_product;
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        cycle_scalar_product = 0;
        for (size_t index = 0; index < vectorSize; ++index) {
            cycle_scalar_product += foo[index] * bar[index];
        }
    }
    timer.stop();

    std::cout << "Cycle scalar product speed: "
              << timer.getSeconds() << std::endl;





    timer.start();
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        foo[0] * bar[0] + foo[1] * bar[1] +
        foo[2] * bar[2] + foo[3] * bar[3] +
        foo[4] * bar[4] + foo[5] * bar[5] +
        foo[6] * bar[6] + foo[7] * bar[7] +
        foo[8] * bar[8] + foo[9] * bar[9] +
        foo[10] * bar[10] + foo[11] * bar[11] +
        foo[12] * bar[12] + foo[13] * bar[13] +
        foo[14] * bar[14] + foo[15] * bar[15] +
        foo[16] * bar[16] + foo[17] * bar[17] +
        foo[18] * bar[18] + foo[19] * bar[19];
    }
    timer.stop();

    std::cout << "Native sum of 20 elements speed: "
              << timer.getSeconds() << std::endl;




    return 0;
}
