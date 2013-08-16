#include <iostream>
#include <string>

class NonCopyable {
public:
    NonCopyable() { }
private:
    NonCopyable(const NonCopyable& other);
    NonCopyable& operator = (const NonCopyable& rhs);
};



template <typename T>
class StaticStack : private NonCopyable {
    size_t sz;
    size_t _top;
    char * memory;
public:
    StaticStack(size_t sz)
        : NonCopyable()
        , sz(sz)
        , _top(0)
        , memory(0)
    {
        memory = new char[sz * sizeof(T)];
    }

    void push_back(const T& elem) {
        if (_top >= sz) {
            throw std::bad_alloc();
        }

        new (memory + _top * sizeof(T)) T(elem);

        ++_top;
    }

    void pop_back() {
        if (isEmpty()) {
            throw std::exception();
        }

        T * elem = (T *)(memory + sizeof(T) * (_top - 1));
        elem->~T();
        --_top;
    }

    size_t size() const {
        return _top;
    }

    bool isEmpty() const {
        return size() == 0;
    }

    const T& top() const {
        if (isEmpty()) {
            throw std::exception("Stack is empty!");
        }

        return *(T *)(memory + sizeof(T) * (_top - 1));
    }

    ~StaticStack() {
        while (!isEmpty()) {
            pop_back();
        }

        delete[] memory;
    }
};

int main() {
    StaticStack<int> foo(5);

    foo.push_back(1);
    foo.push_back(2);
    foo.push_back(3);
    foo.push_back(4);
    foo.push_back(5);

    std::cout << foo.top() << std::endl;
    foo.pop_back();
    std::cout << foo.top() << std::endl;

    return 0;
}
