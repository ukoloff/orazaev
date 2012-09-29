#include<vector>
#include<iostream>
#include<cstdlib>

#include"test_case.h"
#include"aorazaev_z3.h"



using NTestCase::equalArrays;
using NTestCase::equalVectors;
using NTestCase::randomArray;
using NTestCase::randomVector;



void vectorTrivialTest() {
    std::vector<int> v(10, 1);
    std::vector<int> z(10, 1);
    
    for(int i = -20; i <= 20; ++i) {
        rotate(v.begin(), v.end(), 0);
        NTestCase::equalVectors(v, z);
    }
}

void arrayTrivialTest() {
    int v[10] = {1};
    int z[10] = {1};
    
    for(int i = -20; i <= 20; ++i) {
        rotate(v, v + 10, 0);
        equalArrays(v, 10, z, 10);
    }
}


void vectorSimpleTest() {
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

void arraySimpleTest() {
    int a[5] = {0, 1, 2, 3, 4};
    int b[5] = {0, 1, 2, 3, 4};
    int c[5] = {2, 3, 4, 0, 1};

    rotate(a, a + 5, -2);
    equalArrays(a, 5, c, 5);

    rotate(b, b + 5, 3);
    equalArrays(b, 5, c, 5);
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

template <typename T>
T * trivialArrayRotate(const T * a, size_t size, T * const dest, int k) {
    if (k > 0) {
        k %= size;
        k -= size;
    } else {
        while(k <= -((int)size)) {
            k += size;
        }
    }
    k *= -1;

    for(int i = k; i < size; ++i) {
        dest[i - k] = a[i];
    }
    for (int i = 0; i < k; ++i) {
        dest[(size - k) + i] = a[i];
    }
    
    return dest;
}

void vectorStressTest(int N) {
    for(int testNo = 1; testNo <= N; ++testNo) {
        std::vector<int> randomV = randomVector<int>(100, -100, 50);
        int k = rand() % 2000 - 1000;

        std::cout << "Test No " << testNo << std::endl;
        std::vector<int> trueAns = trivialRotate(randomV, k);
        rotate(randomV.begin(), randomV.end(), k);
    
        equalVectors(trueAns, randomV);
    }
}

void arrayStressTest(int N) {
    for(int testNo = 1; testNo <= N; ++testNo) {
        int a[50];
        int b[50];

        size_t array_size = randomArray(a, 100, -100, 50);
        int k = rand() % 2000 - 1000;

        std::cout << "Test No " << testNo << std::endl;
        trivialArrayRotate(a, array_size, b, k);
        rotate(a, a + array_size, k);

        equalArrays(a, array_size, b, array_size);
    }
}

void vectorTestCase() {
    vectorTrivialTest();
    vectorSimpleTest();
    vectorStressTest(10000);
}

void arrayTestCase() {
    arrayTrivialTest();
    arraySimpleTest();
    arrayStressTest(10000);
}



int main() {
    std::srand(360);
    arrayTestCase();
    vectorTestCase();

    return 0;
}
