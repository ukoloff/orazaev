#include <iostream>

template <typename T, size_t SZ>
class MemoryPool {
    static bool isBusy[SZ];
    static T memory[SZ];
public:
    static void * operator new(size_t sz) {
        size_t place = -1;
        for (size_t i = 0; i < SZ; ++i) {
            if (!isBusy[i]) {
                place = i;
                break;
            }
        }

        if (place == -1) {
            std::cout << "Call dinamic new.\n";
            return ::operator new(sz);
        }

        isBusy[place] = true;

        std::cout << "Call static new.\n";
        return (void *) (memory + place /* * sizeof(T) */);
    }

    static void operator delete(void * ptr, size_t sz) {
        size_t place = ((T *) ptr - memory) /* / sizeof(T) */;
        std::cout << "place " << place << "\n";
        if (place < SZ) {
            std::cout << "Call static delete.\n";
            isBusy[place] = false;
        } else {
            std::cout << "Call dinamic delete.\n";
            ::operator delete(ptr);
        }
    }

    virtual ~MemoryPool() {
    }
};

template <typename T, size_t SZ>
bool MemoryPool<T, SZ>::isBusy[SZ] = {false};

template <typename T, size_t SZ>
T MemoryPool<T, SZ>::memory[SZ];


class Foo : public MemoryPool<Foo, 3> {
public:
    int x;
    char y;
    int z;
    char v;

    Foo() {
    }

    Foo(int x, char y)
        : x(x)
        , y(y) {
    }
};


int main() {
    const size_t size = 10;
    Foo * a[size];
    for (int i = 0; i < size; ++i) {
        a[i] = new Foo(i, i + 71);
        std::cout << a[i]->x << " " << a[i]->y << "\n";
    }

    for (int i = 0; i < size; ++i) {
        delete a[i];
    }

    return 0;
}
