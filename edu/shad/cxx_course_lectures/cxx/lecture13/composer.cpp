#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <iterator>

#include <stdlib.h>

template <typename F1, typename F2>
struct Composer
    : public std::unary_function<
        typename F2::argument_type,
        typename F1::result_type
> {
    public:
    F1 f1;
    F2 f2;


    Composer(F1 a, F2 b)
        : f1(a)
        , f2(b) {
    }

    typename F1::result_type operator() (typename F2::argument_type a) const {
        return f1(f2(a));
    }
};


template <typename F1, typename F2>
Composer<F1, F2> compose(F1 f1, F2 f2) {
    return Composer<F1, F2>(f1, f2);
}


const char* toCstr(const std::string& s) {
    return s.c_str();
}


int main() {
    std::string a[] = {
        std::string("0"),
        std::string("23"),
        std::string("34")
    };

    std::transform(
        a,
        a + 3,
        std::ostream_iterator<double>(std::cout, " "),
        compose(std::ptr_fun(atof), std::ptr_fun(toCstr))
    );

    std::cout << std::endl;
    std::transform(
        a,
        a + 3,
        std::ostream_iterator<double>(std::cout, " "),
        compose(std::ptr_fun(atof), std::mem_fun_ref(&std::string::c_str))
    );

}
