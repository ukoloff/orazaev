#ifndef TEST_CASE_H
#define TEST_CASE_H
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>

namespace NTestCase {

    /* Specification here             */
    /* You can find realization above */
    
    class TestFailed {
    public:
        TestFailed();
        TestFailed(const std::string & what);
    };

    template <typename T>
    void printElements(T begin, T end, 
                       std::string space = " ");
 
    template <typename T>
    void equalArrays(const T * a1, size_t size1, 
                 const T * a2, size_t size2);

    template <typename T>
    void equalVectors(const std::vector<T> & v1, 
                      const std::vector<T> & v2);

    template <typename T>
    std::vector<T> randomVector(int maxValue, int minValue, 
                                size_t maxElements);
    
    template <typename T>
    size_t randomArray(T * dest, int maxValue, int minValue, 
                       size_t maxElements);




    /* Realization here */
    TestFailed::TestFailed() {
        std::cout << "TEST FAILED " << std::endl;
    }

    TestFailed::TestFailed(const std::string & what) {
        std::cout << "TEST FAILED: " << what << std::endl;
    }

    template <typename T>
    void printElements(T begin, T end, 
                       std::string space = " ") {
        for(T it = begin; it != end; ++it) {
            std::cout << *it << space;
        }
        std::cout << std::endl;
    }

    template <typename T>
    void equalArrays(const T * a1, size_t size1, 
                 const T * a2, size_t size2) {
        bool equal = 1;
        if (size1 != size2) {
            equal = 0;
        } else {
            for (int i = 0; i < size1; ++i) {
                if (a1[i] != a2[i]) {
                    equal = 0;
                    break;
                }
            }
        }
    
        if (!equal) {
            std::cout << "Arrays are not equal!\n";
            std::cout << "Array 1: ";
            printElements(a1, a1 + size1);
            std::cout << "Array 2: ";
            printElements(a2, a2 + size2);
    
            throw TestFailed();
        }
    }


    template <typename T>
    void equalVectors(const std::vector<T> & v1, 
                      const std::vector<T> & v2) {
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
    std::vector<T> randomVector(int maxValue, int minValue, 
                                size_t maxElements) {
        size_t size = rand() % maxElements + 1;
        
        std::vector<T> randomVector;
        
        while(size-- > 0) {
            randomVector.push_back((T)(rand() % (maxValue - minValue) + minValue));
        }
        
        return randomVector;
    }
    
    template <typename T>
    size_t randomArray(T * dest, int maxValue, int minValue, size_t maxElements) {
        size_t size = rand() % maxElements + 1;
        
        for(int i = 0; i < size; ++i) {
            dest[i] = rand() % (maxValue - minValue) + minValue;
        }
    
        return size;
    }

} /* NTestCase */

#endif /* TEST_CASE_H */
