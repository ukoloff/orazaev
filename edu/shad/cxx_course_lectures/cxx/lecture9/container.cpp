#include <vector>
#include <iostream>

template <typename T, typename Container = std::vector<T> >
class TVector : public Container {
    public:
    TVector() {}

    T operator [] (size_t index) const {
        if (index >= this->size()) {
            return T();
        }

        return this->Container::operator[](index);
    }


    T & operator [] (size_t index) {
        if (index >= this->size()) {
            this->resize(index + 1);
        }

        return this->Container::operator[](index);
    }
};

int main() {
    TVector<int> v;
    std::cout << v.size() << std::endl;
    v.push_back(10);
    std::cout << v[0] << " " << v.size() << std::endl;
    std::cout << v[10] << std::endl;

    v[10] = 23;
    std::cout << v[9] << std::endl;
    std::cout << v[10] << std::endl;

    return 0;
}
