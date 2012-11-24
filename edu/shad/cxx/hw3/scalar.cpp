#include <time.h>
#include <cstdlib>
#include <unistd.h>

#include <iostream>



////////////////////////////// TScalar_product ////////////////////////////////

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
    TScalar_product<vectorSize> scalar_product;
    TTimer timer;


    int foo[vectorSize];
    int bar[vectorSize];
    for (size_t i = 0; i < vectorSize; ++i) {
        foo[i] = bar[i] = 1;
    }




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

    std::cout << "Native scalar product speed: "
              << timer.getSeconds() << std::endl;





    timer.start();
    for (size_t runNo = 0; runNo < numberOfStarts; ++runNo) {
        scalar_product(foo, bar);
    }
    timer.stop();

    std::cout << "Template scalar product speed: "
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

    std::cout << "cycle scalar_product speed: "
              << timer.getSeconds() << std::endl;




    return 0;
}
