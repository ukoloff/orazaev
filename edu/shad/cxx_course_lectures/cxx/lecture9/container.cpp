#include <vector>
#include <iostream>
#include <assert.h>

template <typename T, typename Container = std::vector<T> >
class TVector : public Container {
    public:
    TVector() {}

    const T& operator [] (size_t index) const {
        if (index >= this->size()) {
            return T();
        }

        return this->Container::operator[](index);
    }


    T& operator [] (size_t index) {
        if (index >= this->size()) {
            this->resize(index + 1);
        }

        return this->Container::operator[](index);
    }
};

int main() {
    TVector<int> v;
    v.push_back(10);

    assert(v[0] == 10 && v.size() == 1 && v[10] == 0);

    v[10] = 42;
    assert(v[9] == 0 && v[10] == 42 && v.size() == 11);

    return 0;
}
