#include <iostream>
#include <sstream>
#include <algorithm>




template <typename T>
void printElements(T begin, T end) {
    while (begin != end) {
        std::cout << *begin++ << " ";
    }
    std::cout << std::endl;
}





//////////////////////////////// Set functors /////////////////////////////////

template <typename T>
class TLambdaSetTo {
    T value;
    public:
    TLambdaSetTo(const T& Value)
        : value(Value) {
    }

    T& operator() (T& variable) const {
        variable = value;
    }
};






////////////////////////////// Logical functors ///////////////////////////////

struct TLambdaLess {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

struct TLambdaGreater {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs > rhs;
    }
};

struct TLambdaEqual {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs == rhs;
    }
};

struct TLambdaLessOrEqual {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs <= rhs;
    }
};

struct TLambdaGreaterOrEqual {
    template <typename T>
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs >= rhs;
    }
};






/////////////////////////////// Output functor ////////////////////////////////

struct TLambdaOutput {
    std::ostream& out;
    std::stringstream accumulated;
    public:
    TLambdaOutput(std::ostream& Out)
        : out(Out) {
    }

    TLambdaOutput(const TLambdaOutput& other)
        : out(other.out)
        , accumulated(other.accumulated.str()) {
    }

    template <typename T>
    TLambdaOutput& operator<< (const T& rhs) {
        accumulated << rhs;
        return *this;
    }

    template <typename T>
    std::ostream& operator() (const T& value) {
        out << value << accumulated.str();
        return out;
    }
};







////////////////////////////// Lambda parameters //////////////////////////////

struct TLambdaFirstParameter {
    template <typename T>
    TLambdaSetTo<T> operator = (T Value) {
        return TLambdaSetTo<T>(Value);
    }
};



struct TLambdaSecondParameter {
};






////////////////////////////////// Operators //////////////////////////////////

TLambdaLess operator< (
    const TLambdaFirstParameter& first,
    const TLambdaSecondParameter& second
) {
    return TLambdaLess();
}

TLambdaLess operator> (
    const TLambdaSecondParameter& second,
    const TLambdaFirstParameter& first
) {
    return TLambdaLess();
}

TLambdaGreater operator> (
    const TLambdaFirstParameter& first,
    const TLambdaSecondParameter& second
) {
    return TLambdaGreater();
}

TLambdaGreater operator< (
    const TLambdaSecondParameter& second,
    const TLambdaFirstParameter& first
) {
    return TLambdaGreater();
}

TLambdaEqual operator== (
    const TLambdaFirstParameter& first,
    const TLambdaSecondParameter& second
) {
    return TLambdaEqual();
}

TLambdaEqual operator== (
    const TLambdaSecondParameter& second,
    const TLambdaFirstParameter& first
) {
    return TLambdaEqual();
}

TLambdaLessOrEqual operator<= (
    const TLambdaFirstParameter& first,
    const TLambdaSecondParameter& second
) {
    return TLambdaLessOrEqual();
}

TLambdaLessOrEqual operator>= (
    const TLambdaSecondParameter& second,
    const TLambdaFirstParameter& first
) {
    return TLambdaLessOrEqual();
}

TLambdaGreaterOrEqual operator>= (
    const TLambdaFirstParameter& first,
    const TLambdaSecondParameter& second
) {
    return TLambdaGreaterOrEqual();
}

TLambdaGreaterOrEqual operator<= (
    const TLambdaSecondParameter& second,
    const TLambdaFirstParameter& first
) {
    return TLambdaGreaterOrEqual();
}





TLambdaOutput operator<< (
    std::ostream& out,
    const TLambdaFirstParameter& first
) {
    return TLambdaOutput(out);
}




/////////////////////////////// Demonstration /////////////////////////////////

int main() {
    TLambdaFirstParameter _1;
    TLambdaSecondParameter _2;

    int foo[] = {
        2, 4, 12, 27, 15
    };

    int bar[] = {
        50, 32, 12, 31, 28
    };

    printElements(foo, foo + 5);

    std::for_each(foo, foo + 5, _1 = 100);
    printElements(foo, foo + 5);


    std::sort(bar, bar + 5, _1 < _2);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _2 < _1);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _2 > _1);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _1 > _2);
    printElements(bar, bar + 5);


    std::sort(bar, bar + 5, _1 <= _2);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _2 <= _1);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _2 >= _1);
    printElements(bar, bar + 5);

    std::sort(bar, bar + 5, _1 >= _2);
    printElements(bar, bar + 5);


    std::for_each(foo, foo + 5, std::cout << _1 << ", ");
    std::cout << std::endl;

    return 0;
}
