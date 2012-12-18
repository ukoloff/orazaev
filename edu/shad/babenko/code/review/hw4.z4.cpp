/*
 *  Copyright (c) 2012 Aman Orazaev
 *
 *  Homework 4, Problem 4
 *
 *  Number of similar triangle classes.
 *
 *
 *
 */
#include <time.h>
#include <cassert>
#include <cstdlib>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>








template <typename Iter>
void printElements(Iter begin, Iter end) {
    while (begin != end) {
        std::cout << *begin++ << " ";
    }
    std::cout << std::endl;
}






/////////////////////////////// class THashElement<T> //////////////////////////



template <typename T>
class THashElement {
    std::vector<T> data;

    void append(const T& elem);

    public:
    THashElement();
    explicit THashElement(const T& elem);

    bool has(const T& elem) const;
    bool insert(const T& elem);
    unsigned capacity() const;

    const T& operator[] (size_t index) const;
};

template <typename T>
THashElement<T>::THashElement()
    : data() {
}

template <typename T>
THashElement<T>::THashElement(const T& elem)
    : data() {
    data.push_back(elem);
}

template <typename T>
bool THashElement<T>::has(const T& elem) const {
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == elem) {
            return true;
        }
    }

    return false;
}

template <typename T>
void THashElement<T>::append(const T& elem) {
    data.push_back(elem);
}

template <typename T>
bool THashElement<T>::insert(const T& elem) {
    if (has(elem)) {
        return false;
    }

    append(elem);

    return true;
}

template <typename T>
unsigned THashElement<T>::capacity() const {
    return data.size();
}

template <typename T>
const T& THashElement<T>::operator[] (size_t index) const {
    return data.at(index);
}








/////////////////////////////// class THashSet<K, T, F> //////////////////////


template <typename K, typename F>
class THashSet {
    typename std::vector<THashElement<K> > table;
    F hash;

    public:
    THashSet(size_t size, F Hash);

    bool insert(const K& key);
};

template <typename K, typename F>
THashSet<K, F>::THashSet(size_t size, F Hash)
    : table(size, THashElement<K>())
    , hash(Hash) {
}

template <typename K, typename F>
bool THashSet<K, F>::insert(const K& key) {
    size_t index = hash(key);
    if (index >= table.size()) {
        throw std::out_of_range(
          "THashSet::insert: hash function generated an index out of range.");
    }

    return table[index].insert(key);
}







/////////////////////////////// struct TTriangle ///////////////////////////////


unsigned gcd(unsigned a, unsigned b) {
    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}


struct TTriangle {
    unsigned side[3];

    TTriangle();
    explicit TTriangle(std::vector<unsigned> sides);
};

TTriangle::TTriangle() {
    side[0] = side[1] = side[2] = 0;
}

TTriangle::TTriangle(std::vector<unsigned> sides) {
    std::sort(sides.begin(), sides.end());
    unsigned divisior = gcd(sides[2], gcd(sides[0], sides[1]));

    for (size_t i = 0; i < 3; ++i) {
        side[i] = sides[i] / divisior;
    }
}

bool operator == (const TTriangle& left, const TTriangle& right) {
    for (size_t i = 0; i < 3; ++i) {
        if (left.side[i] != right.side[i]) {
            return false;
        }
    }

    return true;
}

bool operator != (const TTriangle& left, const TTriangle& right) {
    return !(left == right);
}

std::ostream& operator<< (std::ostream& out, const TTriangle& triangle) {
    out << "["
        << triangle.side[0] << ", "
        << triangle.side[1] << ", "
        << triangle.side[2] << "]";
    return out;
}

bool operator< (const TTriangle& left, const TTriangle& right) {
    for (size_t i = 0; i < 3; ++i) {
        if (left.side[i] == right.side[i])
            continue;

        if (left.side[i] < right.side[i])
            return true;
        else
            return false;
    }

    return false;
}








/////////////////////////////// struct TTriangleHashFunction ///////////////////

static const size_t PRIME = 3000017;



size_t generateRandom(size_t from, size_t to) {
    return rand() % ((to + 1) - from) + from;
}



struct TTriangleHashFunction {
    std::vector<size_t> coefficient;

    TTriangleHashFunction()
        : coefficient(3, 0) {
        for (size_t i = 0; i < 3; ++i) {
            coefficient[i] = generateRandom(1, PRIME - 1);
        }
    }

    size_t operator() (const TTriangle& triangle) {
        size_t result = 0;

        for (size_t i = 0; i < 3; ++i) {
            result += static_cast<size_t>(
                static_cast<unsigned long>(
                    triangle.side[i]) *
                coefficient[i] % PRIME);
        }

        return result % PRIME;
    }
};







/////////////////////////////// size_t numberOfClasses  ////////////////////////

std::vector<TTriangle> readData() {
    std::vector<TTriangle> result;
    size_t size;
    std::cin >> size;
    result.reserve(size);

    std::vector<unsigned> triangle(3, 0);

    for (size_t i = 0; i < size; ++i) {
        std::cin >> triangle[0] >> triangle[1] >> triangle[2];

        result.push_back(TTriangle(triangle));
    }

    assert(result.size() == size);

    return result;
}


size_t numberOfClasses(std::vector<TTriangle>* pData) {
    size_t ans;

    ans = 0;

    typedef THashSet<TTriangle, TTriangleHashFunction>
        TTriangleHashSet;
    TTriangleHashSet hashSet(PRIME, TTriangleHashFunction());


    for (size_t i = 0; i < pData->size(); ++i) {
        if (hashSet.insert((*pData)[i])) {
            ++ans;
        }
    }

    return ans;
}





int main() {
    std::srand(360);
    std::vector<TTriangle> data = readData();
    std::cout << numberOfClasses(&data);

    return 0;
}
