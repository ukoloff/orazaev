#include <functional>
#include <iostream>


template <typename C>
struct FunType {
    typedef typename C::result_type result_type;
    typedef typename C::argument_type argument_type;
};

template <typename Res, typename Arg>
struct FunType<Res(Arg)> {
    typedef Res result_type;
    typedef Arg argument_type;
};



template <typename Res, typename Arg>
class FunctionImpl {
    public:
    virtual Res operator() (Arg) = 0;

    virtual ~FunctionImpl() {
    }
};




template <typename Res, typename Arg>
class FunctionImplPtr : public FunctionImpl<Res, Arg> {
    Res (*fun)(Arg);

    public:
    FunctionImplPtr(Res (*Fun)(Arg))
        : fun(Fun) {
    }

    Res operator() (Arg argument) {
        return fun(argument);
    }
};




template <typename F>
class FunctionImplFunctor
    : public FunctionImpl<typename F::result_type, typename F::argument_type> {
    F fun;

    public:
    FunctionImplFunctor(F Fun)
        : fun(Fun) {
    }

    typename F::result_type operator() (typename F::argument_type argument) {
        return fun(argument);
    }
};



template <typename Res, typename Arg>
FunctionImpl<Res, Arg>* createFunctionImpl(Res (*f)(Arg)) {
    return new FunctionImplPtr<Res, Arg>(f);
}



template <typename F>
FunctionImpl<
    typename F::result_type,
    typename F::argument_type
>* createFunctionImpl(F f) {
    return new FunctionImplFunctor<F>(f);
}




template <typename F>
class Function {
    public:
    typedef typename FunType<F>::result_type result_type;
    typedef typename FunType<F>::argument_type argument_type;

    Function(F* fun) {
        pImpl = new FunctionImplPtr<result_type, argument_type>(fun);
    }


    template <typename T>
    Function(T fun) {
        pImpl = new FunctionImplFunctor<T>(fun);
    }


    result_type operator() (argument_type argument) {
        return pImpl->operator()(argument);
    }

    ~Function() {
        delete pImpl;
    }

    private:
    FunctionImpl<result_type, argument_type>* pImpl;
};




int some_func(int a) {
    return a + 1;
}

struct Some_func : public std::unary_function<int, int> {
    int operator() (int a) {
        return a + 2;
    }
};


int main() {
    Function<int(int)> fun = some_func;

    std::cout << fun(1) << std::endl;

    Function<int(int)> fun2 = Some_func();

    std::cout << fun2(1) << std::endl;
    return 0;
}
