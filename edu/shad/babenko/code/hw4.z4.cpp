/*
 *  Copyright (c) 2012 Aman Orazaev
 *
 *  Homework 4, Problem 4
 *
 *  Number of similar triangle classes.
 *
 *
 *  TODO: code style fixes
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
    unsigned dataSize;
    T* data;

    void append(const T& elem);

    public:
    THashElement();
    explicit THashElement(const T& elem);
    THashElement(const THashElement& he);

    THashElement& operator = (const THashElement& he);

    bool has(const T& elem) const;
    bool insert(const T& elem);
    unsigned capacity() const;

    const T& operator[] (size_t index) const;
    T& operator[] (size_t index);

    ~THashElement();
};

template <typename T>
THashElement<T>::THashElement()
    : dataSize(0)
    , data(0) {
}

template <typename T>
THashElement<T>::THashElement(const T& elem) {
    data = new T[1];
    data[0] = elem;

    dataSize = 1;
}

template <typename T>
THashElement<T>::THashElement(const THashElement<T>& he) {
    this->data = new T[he.dataSize];

    this->dataSize = he.dataSize;

    for (size_t i = 0; i < he.dataSize; ++i) {
        // FIXME: Not safe for complecated operator = exception
        this->data[i] = he.data[i];
    }
}

template <typename T>
THashElement<T>& THashElement<T>::operator = (const THashElement<T>& he) {
    if (&he == this) {
        return *this;
    }

    delete [] data;
    this->data = new T[he.dataSize];

    this->dataSize = he.dataSize;

    for (size_t i = 0; i < he.dataSize; ++i) {
        // FIXME: Not safe for complecated operator = exception
        this->data[i] = he.data[i];
    }

    return *this;
}

template <typename T>
bool THashElement<T>::has(const T& elem) const {
    for (size_t i = 0; i < dataSize; ++i) {
        if (data[i] == elem) {
            return true;
        }
    }

    return false;
}

template <typename T>
void THashElement<T>::append(const T& elem) {
    T * newData = new T[dataSize + 1];
    for (size_t i = 0; i < dataSize; ++i) {
        newData[i] = data[i];
    }
    newData[dataSize] = elem;

    delete [] data;
    data = newData;

    ++dataSize;
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
    return dataSize;
}

template <typename T>
const T& THashElement<T>::operator[] (size_t index) const {
    if (index >= dataSize) {
        throw std::out_of_range("THashElement: index is out of range.");
    }

    return data[index];
}

template <typename T>
T& THashElement<T>::operator[] (size_t index) {
    if (index >= dataSize) {
        throw std::out_of_range("THashElement: index is out of range.");
    }

    return data[index];
}

template <typename T>
THashElement<T>::~THashElement() {
    delete [] data;
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

struct TTriangle {
    unsigned side[3];

    TTriangle();
    explicit TTriangle(std::vector<unsigned> sides);

    unsigned gcd(unsigned a, unsigned b);
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

unsigned TTriangle::gcd(unsigned a, unsigned b) {
    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}

bool operator == (TTriangle left, TTriangle right) {
    for (size_t i = 0; i < 3; ++i) {
        if (left.side[i] != right.side[i]) {
            return false;
        }
    }

    return true;
}

bool operator != (TTriangle left, TTriangle right) {
    return !(left == right);
}

std::ostream& operator<< (std::ostream& out, const TTriangle & triangle) {
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
    static std::vector<TTriangle> data;

    unsigned index;

    TIndex() {}
    explicit TIndex(unsigned Index);
};

std::vector<TTriangle> TIndex::data;

TIndex::TIndex(unsigned Index)
    : index(Index) {
}

bool operator == (TIndex left, TIndex right) {
    return TIndex::data[left.index] == TIndex::data[right.index];
}

bool operator != (TIndex left, TIndex right) {
    return !(left == right);
}

std::ostream& operator << (std::ostream& out, const TIndex& index) {
    out << "i" << index.index;
    return out;
}








/////////////////////////////// struct TTriangleHashFunction ///////////////////

// static const size_t PRIME = 1000003;
static const size_t PRIME = 3000017;


struct TTriangleHashFunction {
    size_t coefficient[3];

    TTriangleHashFunction() {
        for (size_t i = 0; i < 3; ++i) {
            coefficient[i] = (rand() % (PRIME - 1)) + 1;
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







/////////////////////////////// void numberOfClasses  //////////////////////////

void readData() {
    size_t size;
    std::cin >> size;
    TIndex::data.reserve(size);

    std::vector<unsigned> triangle(3, 0);

    for (size_t i = 0; i < size; ++i) {
        std::cin >> triangle[0] >> triangle[1] >> triangle[2];

        TIndex::data.push_back(TTriangle(triangle));
    }

    assert(TIndex::data.size() == size);

    // printElements(TIndex::data.begin(), TIndex::data.end());
}


size_t numberOfClasses() {
    size_t ans;
    bool tableWasCreated = true;

    do {
        ans = 0;

        typedef THashTable<TTriangle, TIndex, TTriangleHashFunction>
            TTriangleHashTable;
        TTriangleHashTable hashTable(PRIME, TTriangleHashFunction());


        for (size_t i = 0; i < TIndex::data.size(); ++i) {
            if (hashTable.insert(TIndex::data[i], TIndex(i))) {
                ++ans;
                if (hashTable.getSquaredSizes() > 3 * PRIME) {
                    tableWasCreated = false;
                    break;
                }
            }
        }
    } while (!tableWasCreated);

    return ans;
}







////////////////////////////////// Testing /////////////////////////////////////


void test_THashElement_memory() {
    THashElement<int> foo(10);
    THashElement<int> bar(foo);
    THashElement<int> baz;
    THashElement<int> qux(baz);

    foo = baz;
    baz = bar;
}


void test_THashElement() {
    test_THashElement_memory();
}


static const size_t testHashSize = 20;
static const size_t testHashPrime = 23;


struct TTestFunctor {
    TTestFunctor() {}
    size_t operator() (int key) {
        return static_cast<int>(
            ((static_cast<long>(key) * 12 + 14) % testHashPrime) %
            testHashSize);
    }
};


void test_THashTable_constuction() {
    THashTable<int, char, TTestFunctor> foo(20, TTestFunctor());
    assert(foo.insert(0, 'a') == true);
    foo.insert(1, 'b');
    foo.insert(2, 'c');
    assert(foo.insert(3, 'c') == true);
}


void test_THashTable_insertion() {
    THashTable<int, char, TTestFunctor> foo(20, TTestFunctor());
    assert(foo.insert(0, 'a') == true);
    foo.insert(1, 'b');
    foo.insert(2, 'c');
    assert(foo.insert(3, 'c') == true);
}


void test_THashTable() {
    test_THashTable_constuction();
    test_THashTable_insertion();
}


void test_TIndex_equal() {
    std::vector<unsigned> foo;
    foo.push_back(1);
    foo.push_back(2);
    foo.push_back(3);

    std::vector<unsigned> qux;
    qux.push_back(1);
    qux.push_back(3);
    qux.push_back(3);

    TIndex::data.push_back(TTriangle(foo));
    foo[0] *= 4;
    foo[1] *= 4;
    foo[2] *= 4;
    TIndex::data.push_back(TTriangle(foo));
    TIndex::data.push_back(TTriangle(qux));

    TIndex bar(0);
    TIndex baz(1);

    assert(bar == baz);

    baz.index = 2;
    assert(bar != baz);
}


void test_TIndex() {
    assert(sizeof(TIndex) == 4);

    test_TIndex_equal();
}


void test_TTriangle_equal() {
    std::vector<unsigned> foo;
    foo.push_back(30);
    foo.push_back(20);
    foo.push_back(40);

    std::vector<unsigned> bar;
    bar.push_back(20);
    bar.push_back(20);
    bar.push_back(666);

    std::vector<unsigned> qux;
    qux.push_back(4);
    qux.push_back(2);
    qux.push_back(3);

    TTriangle Foo(foo);
    TTriangle Bar(bar);
    TTriangle Qux(qux);

    assert(Foo == Qux);
    assert(Bar != Foo);
}


// corrupt TIndex::data
size_t trivialNumberOfClasses() {
    std::sort(TIndex::data.begin(), TIndex::data.end());
    std::vector<TTriangle>::iterator it =
        std::unique(TIndex::data.begin(), TIndex::data.end());

    return it - TIndex::data.begin();
}


void assertEqual(size_t first, size_t second) {
    if (first != second) {
        std::cerr << "assertion failed:" << std::endl
                  << "first:  " << first << std::endl
                  << "second: " << second << std::endl;

        abort();
    }
}


TTriangle randomTriangle(size_t maxSideLength) {
    std::vector<unsigned> sideVector(3, 0);
    for (size_t i = 0; i < 3; ++i) {
        sideVector[i] = (rand() % maxSideLength) + 1;
    }

    return TTriangle(sideVector);
}


void createRandomData(size_t maxDataSize, size_t maxSideLength) {
    size_t dataSize = rand() % maxDataSize + 1;
    dataSize = maxDataSize;
    TIndex::data.resize(dataSize);

    for (size_t i = 0; i < dataSize; ++i) {
        TIndex::data[i] = randomTriangle(maxSideLength);
    }
}


void stressTesting(size_t numberOfTests) {
    size_t testNo = 0;
    while (testNo++ < numberOfTests) {
        std::cout << "Test No: " << testNo << std::endl;
        createRandomData(1000000, 200);
        std::cout << "Data size: " << TIndex::data.size() << std::endl;


        clock_t start = clock();
        size_t ans = numberOfClasses();
        clock_t end = clock();


        double time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        std::cout << "TIME: " << time << std::endl;
        assert(time < 1.0);


        start = clock();
        size_t trivialAns = trivialNumberOfClasses();
        end = clock();
        std::cout << "STL_TIME: "
                  << static_cast<double>(end - start) / CLOCKS_PER_SEC
                  << std::endl;


        assertEqual(ans, trivialAns);


        std::cout << "Answer: " << ans << std::endl
                  << "-----------------------------" << std::endl;
    }
}



int main() {
    std::srand(360);
    // test_THashElement();
    // test_THashTable();
    // test_TIndex();
    // test_TTriangle_equal();
    // stressTesting(100);
    readData();
    std::cout << numberOfClasses();

    return 0;
}
