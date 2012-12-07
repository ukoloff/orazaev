#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>




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

struct TLambdaNot {
    template <typename T>
    bool operator() (const T& rhs) {
        return !rhs;
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







////////////////////////// Lambda @ Value Functors ////////////////////////////

template <typename T>
class TLambdaPlusValue {
    T value;
    public:
    TLambdaPlusValue(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A operator()(const A& variable) {
        return variable + value;
    }
};

template <typename T>
class TLambdaMinusValue {
    T value;
    public:
    TLambdaMinusValue(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A operator()(const A& variable) {
        return variable - value;
    }
};

template <typename T>
class TLambdaTimesValue {
    T value;
    public:
    TLambdaTimesValue(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A operator()(const A& variable) {
        return variable * value;
    }
};

template <typename T>
class TLambdaDivValue {
    T value;
    public:
    TLambdaDivValue(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A operator()(const A& variable) {
        return variable / value;
    }
};

template <typename T>
class TLambdaModValue {
    T value;
    public:
    TLambdaModValue(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A operator()(const A& variable) {
        return variable % value;
    }
};







////////////////////////// Lambda @= Value Functors ///////////////////////////

template <typename T>
class TLambdaEqualPlus {
    T value;
    public:
    TLambdaEqualPlus(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A& operator() (A& variable) {
        variable += value;
        return variable;
    }
};

template <typename T>
class TLambdaEqualMinus {
    T value;
    public:
    TLambdaEqualMinus(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A& operator() (A& variable) {
        variable -= value;
        return variable;
    }
};

template <typename T>
class TLambdaEqualTimes {
    T value;
    public:
    TLambdaEqualTimes(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A& operator() (A& variable) {
        variable *= value;
        return variable;
    }
};

template <typename T>
class TLambdaEqualDiv {
    T value;
    public:
    TLambdaEqualDiv(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A& operator() (A& variable) {
        variable /= value;
        return variable;
    }
};

template <typename T>
class TLambdaEqualMod {
    T value;
    public:
    TLambdaEqualMod(const T& Value)
        : value(Value) {
    }

    template <typename A>
    A& operator() (A& variable) {
        variable %= value;
        return variable;
    }
};









/////////////////////////////// @Lambda Functors //////////////////////////////

struct TLambdaUnaryMinus {
    template <typename T>
    T operator() (const T& variable) {
        return -variable;
    }
};

struct TLambdaUnaryPlus {
    template <typename T>
    T operator() (const T& variable) {
        return +variable;
    }
};

struct TLambdaUnaryStar {
    template <typename T>
    T operator() (const T* const variable) {
        return *variable;
    }
};

struct TLambdaPrefixIncrement {
    template <typename T>
    T operator() (T& variable) {
        return ++variable;
    }
};

struct TLambdaPostfixIncrement {
    template <typename T>
    T operator() (T& variable) {
        return variable++;
    }
};

struct TLambdaPrefixDecrement {
    template <typename T>
    T operator() (T& variable) {
        return --variable;
    }
};

struct TLambdaPostfixDecrement {
    template <typename T>
    T operator() (T& variable) {
        return variable--;
    }
};






////////////////////////////// Lambda parameters //////////////////////////////

struct TLambdaFirstParameter {
    template <typename T>
    TLambdaSetTo<T> operator = (T Value) {
        return TLambdaSetTo<T>(Value);
    }

    template <typename T>
    TLambdaPlusValue<T> operator+ (const T& value) {
        return TLambdaPlusValue<T>(value);
    }

    TLambdaUnaryPlus operator+ () {
        return TLambdaUnaryPlus();
    }

    template <typename T>
    TLambdaMinusValue<T> operator- (const T& value) {
        return TLambdaMinusValue<T>(value);
    }

    TLambdaUnaryMinus operator-() {
        return TLambdaUnaryMinus();
    }

    template <typename T>
    TLambdaTimesValue<T> operator* (const T& value) {
        return TLambdaTimesValue<T>(value);
    }

    TLambdaUnaryStar operator* () {
        return TLambdaUnaryStar();
    }

    template <typename T>
    TLambdaDivValue<T> operator/ (const T& value) {
        return TLambdaDivValue<T>(value);
    }

    template <typename T>
    TLambdaModValue<T> operator% (const T& value) {
        return TLambdaModValue<T>(value);
    }

    template <typename T>
    TLambdaEqualPlus<T> operator+= (const T& value) {
        return TLambdaEqualPlus<T>(value);
    }

    template <typename T>
    TLambdaEqualMinus<T> operator-= (const T& value) {
        return TLambdaEqualMinus<T>(value);
    }

    template <typename T>
    TLambdaEqualTimes<T> operator*= (const T& value) {
        return TLambdaEqualTimes<T>(value);
    }

    template <typename T>
    TLambdaEqualDiv<T> operator /= (const T& value) {
        return TLambdaEqualDiv<T>(value);
    }

    template <typename T>
    TLambdaEqualMod<T> operator %= (const T& value) {
        return TLambdaEqualMod<T>(value);
    }

    TLambdaPrefixIncrement operator++ () {
        return TLambdaPrefixIncrement();
    }

    TLambdaPrefixDecrement operator-- () {
        return TLambdaPrefixDecrement();
    }

    TLambdaPostfixIncrement operator++ (int) {
        return TLambdaPostfixIncrement();
    }

    TLambdaPostfixDecrement operator-- (int) {
        return TLambdaPostfixDecrement();
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

TLambdaNot operator! (const TLambdaFirstParameter& first) {
    return TLambdaNot();
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
        50, 32, 0, 31, 28
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




    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<bool>(std::cout, " "),
        !_1
    );
    std::cout << std::endl;




    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1 / 2
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1 + 1000
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1 * 10
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1 - 100
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1 % 10
    );
    std::cout << std::endl;




    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        -_1
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        +_1
    );
    std::cout << std::endl;




    int* baz[5];
    baz[0] = bar;
    baz[1] = bar + 1;
    baz[2] = bar + 2;
    baz[3] = bar + 3;
    baz[4] = bar + 4;

    std::transform(
        baz,
        baz + 5,
        std::ostream_iterator<int>(std::cout, " "),
        *_1
    );
    std::cout << std::endl;




    std::for_each(bar, bar + 5, _1 += 100);
    printElements(bar, bar + 5);

    std::for_each(bar, bar + 5, _1 -= 100);
    printElements(bar, bar + 5);

    std::for_each(bar, bar + 5, _1 *= 10);
    printElements(bar, bar + 5);

    std::for_each(bar, bar + 5, _1 /= 10);
    printElements(bar, bar + 5);

    std::for_each(foo, foo + 5, _1 %= 13);
    printElements(foo, foo + 5);




    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1++
    );
    std::cout << std::endl;
    printElements(bar, bar + 5);

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        --_1
    );
    std::cout << std::endl;

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        _1--
    );
    std::cout << std::endl;
    printElements(bar, bar + 5);

    std::transform(
        bar,
        bar + 5,
        std::ostream_iterator<int>(std::cout, " "),
        ++_1
    );
    std::cout << std::endl;



    return 0;
}
