#include <iostream>
#include <typeinfo>
#include <memory>
#include <vector>

template <typename T>
class TAllocator {
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

    pointer address (reference r) const {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::address(" << r
                  << ") return " << &r << "\n";
        return &r;
    }

    const_pointer address(const_reference r) const {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::address(" << r
                  << ") return " << &r << "\n";
        return &r;
    }

    TAllocator() throw() {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::TAllocator()\n";
    }

    template <class U>
    TAllocator(const TAllocator<U>& other) throw() {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::TAllocator(const TAllocator<" << typeid(U).name()
                  << ">)\n";
    }

    ~TAllocator() {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::~TAllocator()\n";
    }

    pointer allocate(size_type n) {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::allocate(" << n <<")\n";

        int index = -1;
        bool box = false;
        for (int i = 0; i < buffer_size; ++i) {
            if (box) {
                if ((i - index + 1) == n) {
                    for (int t = index; t < i; ++t) {
                        isBusy[t] = false;
                    }
                    return staticBuffer + index;
                }
            }

            if (!isBusy[i]) {
                index = i;
                box = true;
            }
        }
    }

    void deallocate(pointer p, size_type n) {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::deallocate(" << p
                  << ", " << n << ")\n";

        delete[] p;
    }

    void construct(pointer p, const T& val) {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::construct(" << p
                  << ", " << val << ")\n";
        new(p) T(val);
    }

    void destroy(pointer p) {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::destroy(" << p << ")\n";
        ~(*p);
    }

    size_type max_size() const throw() {
        std::cout << "TAllocator<" << typeid(T).name()
                  << ">::max_size()\n";
        return buffer_size;
    }

    template <class U>
    struct rebind { typedef TAllocator<U> other; };

private:
    static const size_type buffer_size = 1000;
    static value_type staticBuffer[buffer_size];
    static bool isBusy[buffer_size];
};

template <typename T>
TAllocator<T>::value_type TAllocator<T>::staticBuffer[TAllocator<T>::buffer_size];

template <typename T>
bool TAllocator<T>::isBusy[TAllocator<T>::buffer_size] = {false};

int main() {
    std::vector<int, TAllocator<int> > vec;

    vec.push_back(10);
    vec.push_back(666);

    return 0;
}
