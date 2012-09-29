/*
 *      C++ homework. Problem 3.
 *      
 *      Due date: 04.09.12
 *
 *      Author: Aman Orazaev
 */
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>

#include"test_case.h"

using std::cout;
using std::endl;
using std::string;



template <typename T>
void shift(T begin, T end) {
    if (begin == end)
        return;

    for (T it = end - 1; it != begin; --it) {
        *(it + 1) = *it;
    }
    *(begin + 1) = *begin;
    *begin = *end;
}

template <typename T>
void rotate(T begin, T end, int k) {
    if (begin == end)
        return;
    
    while (k < 0)
        k += end - begin;

    while(k-- > 0)
        shift(begin, end);
}




template <typename T>
void printElements(T begin, T end, string space = " ") {
    for(T it = begin; it != end; ++it) {
        cout << *it << space;
        
    }
    cout << endl;
}

template <typename T>
void equalVectors(const std::vector<T> & v1, const std::vector<T> & v2) {
    if (v1 != v2) {
        std::cout << "Vectors are not equal!\n";
        std::cout << "Vector 1: ";
        printElements(v1.begin(), v1.end());
        std::cout << "Vector 2: ";
        printElements(v2.begin(), v2.end());
        
        throw TestFailed();
    }
}

template <typename T>
void equalArrays(const T * a1, size_t size1, 
                 const T * a2, size_t size2) {
    
    
}

void trivialTest() {
    std::vector<int> v(10, 1);
    std::vector<int> z(10, 1);
    
    for(int i = -20; i <= 20; ++i) {
        rotate(v.begin(), v.end(), 0);
        equalVectors(v, z);
    }
}

void arrayTrivialTest() {
    v[10] = {1};
    z[10] = {1};
    
    for(int i = -20; i <= 20; ++i) {
        rotate(v.begin(), v.end(), 0);
        equalVectors(v, z);
    }
}


void simpleTest() {
    std::vector<int> v;
    for(int i = 0; i < 5; ++i)
        v.push_back(i);

    std::vector<int> v2;
    for(int i = 0; i < 5; ++i)
        v2.push_back(i);

    std::vector<int> z;
    for(int i = 2; i < 5; ++i)
        z.push_back(i);
    z.push_back(0);
    z.push_back(1);
    


    rotate(v.begin(), v.end(), -2);
    equalVectors(v, z);
    
    rotate(v2.begin(), v2.end(), 3);
    equalVectors(v2, z);
}

template <typename T>
std::vector<T> randomVector(int maxValue, int minValue, size_t maxElements) {
    size_t size = rand() % maxElements + 1;
    
    std::vector<T> randomVector;
    
    while(size-- > 0) {
        randomVector.push_back((T)(rand() % (maxValue - minValue) + minValue));
    }
    
    return randomVector;
}

template <typename T>
std::vector<T> trivialRotate(const std::vector<T> & v, int k) {
    if (k > 0) {
        k %= v.size();
        k -= v.size();
    } else {
        while(k <= -((int)v.size())) {
            k += v.size();
        }
    }
    k *= -1;
    

    std::vector<T> ans;
    
    for(int i = k; i < v.size(); ++i) {
        ans.push_back(v[i]);
    }
    for(int i = 0; i < k; ++i) {
        ans.push_back(v[i]);
    }

    return ans;
}

void stressTest(int N) {
    for(int testNo = 1; testNo <= N; ++testNo) {

        std::vector<int> randomV = randomVector<int>(100, -100, 50);
        int k = rand() % 2000 - 1000;
        cout << "Test No " << testNo << endl;

        std::vector<int> trueAns = trivialRotate(randomV, k);
        rotate(randomV.begin(), randomV.end(), k);
    
        equalVectors(trueAns, randomV);
    }
}

void vectorTestCase() {
    trivialTest();
    simpleTest();
    stressTest(10000);
}

void arrayTestCase() {

}


int main() {
    std::srand(360);
    vectorTestCase();

    return 0;
}
