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








/////////////////////////////// class THashTable<K, T, F> //////////////////////


template <typename K, typename T, typename F>
class THashTable {
    THashElement<T> * table;
    size_t tableSize;
    F hash;
    unsigned long squaredSizes;

    THashTable(const THashTable&);
    THashTable& operator = (const THashTable&);

    public:
    THashTable(size_t size, F Hash);

    bool insert(const K& key, const T& value);

    unsigned long getSquaredSizes() const;

    ~THashTable();
};

template <typename K, typename T, typename F>
THashTable<K, T, F>::THashTable(size_t size, F Hash)
    : table(0)
    , tableSize(size)
    , hash(Hash)
    , squaredSizes(0) {
    table = new THashElement<T>[tableSize];
}

template <typename K, typename T, typename F>
bool THashTable<K, T, F>::insert(const K& key, const T& value) {
    size_t index = hash(key);
    if (index >= tableSize) {
        throw std::out_of_range(
          "THashTable::insert: hash function generated an index out of range.");
    }

    squaredSizes -= static_cast<unsigned long>(
        table[index].capacity()) * table[index].capacity();
    squaredSizes += static_cast<unsigned long>(
        table[index].capacity() + 1) * (table[index].capacity() + 1);

    return table[index].insert(value);
}

template <typename K, typename T, typename F>
unsigned long THashTable<K, T, F>::getSquaredSizes() const {
    return squaredSizes;
}

template <typename K, typename T, typename F>
THashTable<K, T, F>::~THashTable() {
    delete [] table;
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







/////////////////////////////// struct TIndex ////////////////////////////////

struct TIndex {
    std::vector<TTriangle>* pData;

    unsigned index;

    TIndex() {}
    explicit TIndex(std::vector<TTriangle>* pData, unsigned Index);
};

TIndex::TIndex(std::vector<TTriangle>* pData, unsigned index)
    : pData(pData)
    , index(index) {
}

bool operator == (const TIndex& left, const TIndex& right) {
    return left.pData == right.pData &&
           (*left.pData)[left.index] == (*right.pData)[right.index];
}

bool operator != (const TIndex& left, const TIndex& right) {
    return !(left == right);
}

std::ostream& operator << (std::ostream& out, const TIndex& index) {
    out << "i" << index.index;
    return out;
}









/////////////////////////////// struct TTriangleHashFunction ///////////////////

static const size_t PRIME = 3000017;


struct TTriangleHashFunction {
    std::vector<size_t> coefficient;

    TTriangleHashFunction()
        : coefficient(3, 0) {
        for (size_t i = 0; i < 3; ++i) {
            coefficient[i] = rand() % PRIME;
            if (coefficient[i] == 0) {
                --i;
            }
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

    typedef THashTable<TTriangle, TIndex, TTriangleHashFunction>
        TTriangleHashTable;
    TTriangleHashTable hashTable(PRIME, TTriangleHashFunction());


    for (size_t i = 0; i < pData->size(); ++i) {
        if (hashTable.insert((*pData)[i], TIndex(pData, i))) {
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
