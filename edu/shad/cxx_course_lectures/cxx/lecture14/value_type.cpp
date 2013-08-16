


template <typename T>
struct HasValueType : public T {
    typedef typename T::value_type foo;
};


struct WithValueType {
    protected:
    typedef int value_type;
};

struct WithoutValueType {
};


template <typename T>
class MyClass: private HasValueType<T> {
};

int main() {
    MyClass<WithValueType> a;
    //MyClass<WithoutValueType> b;

    return 0;
}
