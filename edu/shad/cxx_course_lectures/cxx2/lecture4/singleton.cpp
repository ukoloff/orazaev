#include <iostream>

class TNonCopyable {
protected:
    TNonCopyable() { }
private:
    TNonCopyable(const TNonCopyable&);
    TNonCopyable& operator=(const TNonCopyable&);
};

template <typename T>
class TSingleton : private TNonCopyable {
public:
    static T& GetInstance() {
        static T instance;
        return instance;
    }

protected:
    TSingleton() { };
};

class TFoo : public TSingleton<TFoo> {
public:
    void Set(int other) {
        x = other;
    }

    int Get() const {
        return x;
    }
private:
    int x;
protected:
    TFoo() { }
    friend class TSingleton<TFoo>;
};



int main() {
    typedef TSingleton<TFoo> TFooSingleton;
    std::cout << TFooSingleton::GetInstance().Get() << std::endl;

    TFooSingleton::GetInstance().Set(666);
    std::cout << TFooSingleton::GetInstance().Get() << std::endl;

    std::cout << TFoo::GetInstance().Get() << std::endl;

    TFoo f;

    return 0;
}
