#include<iostream>
#include<vector>
#include<stdexcept>
#include<time.h>
#include<cstdlib>

template <typename A>
void printElements(A begin, A end, std::string space = " ");

size_t max(size_t x, size_t y) {
    return x > y ? x : y;
}

/* Larges Common Subsequence size */
template <typename A, typename B>
size_t sizeOfLCS(A begin0, A end0, B begin1, B end1) {

    size_t seqSize0 = end0 - begin0;
    size_t seqSize1 = end1 - begin1;

    std::vector<size_t> newL[2] = {
        std::vector<size_t>(seqSize1 + 1, 0),
        std::vector<size_t>(seqSize1 + 1, 0)
    };
    int line = 0;

    for(size_t i0 = 0; i0 < seqSize0; ++i0) {
        for (size_t i1 = 0; i1 < seqSize1; ++i1) {
            if (*(begin0 + i0) == *(begin1 + i1)) {
                newL[line][i1 + 1] = newL[(line + 1) % 2][i1] + 1;
            } else {
                newL[line][i1 + 1] =
                    max(newL[(line + 1) % 2][i1 + 1], newL[line][i1]);
            }
        }
        line = (line + 1) % 2;
    }

    return newL[(line + 1) % 2][seqSize1];
}




/* Tests for sizeOfLCS */
template <typename A>
void printElements(A begin, A end, std::string space = " ") {

    while(begin != end) {
        std::cout << *begin++ << space;
    }

    std::cout << std::endl;
}

template <typename A, typename B>
double sizeOfLcsIs(A begin0, A end0, B begin1, B end1, size_t size) {

    clock_t start = clock();
    size_t result = sizeOfLCS(begin0, end0, begin1, end1);
    clock_t end = clock();
    if (result != size) {
        std::cout << std::endl << "=============================="
            << std::endl;
        std::cout << "Sequence0: ";
        printElements(begin0, end0);
        std::cout << "Sequence1: ";
        printElements(begin1, end1);

        std:: cout << "Result of sizeOfLCS() = "
            << result << std::endl;
        std:: cout << "Expected result       = "
            << size << std::endl;
        throw std::runtime_error("TEST FAILED!");
    }
    return (double) (end - start) / CLOCKS_PER_SEC;
}



void simpleTest() {
    int seq0[5] = {1, 2, 3, 4, 5};
    int seq1[5] = {1, 3, 5, 7, 9};

    std::cout << "Simple test... ";
    double time =
        sizeOfLcsIs(seq0, seq0 + 5, seq1, seq1 + 5, 3);
    std::cout << " " << time << " secondes" << std::endl;
}

void emptyTest() {
    std::vector<int> seq0;
    std::vector<int> seq1;

    std::cout << "Empty test...";
    double time =
        sizeOfLcsIs(seq0.begin(), seq0.end(), seq1.begin(), seq1.end(), 0);
    std::cout << " " << time << " secondes" << std::endl;
}

void trivialTest(int a, int b, size_t r, int n) {
    std::vector<int> seq0(1, a);
    std::vector<int> seq1(1, b);

    std::cout << "Trivial test" << n << "...";
    double time =
        sizeOfLcsIs(seq0.begin(), seq0.end(), seq1.begin(), seq1.end(), r);
    std::cout << " " << time << " secondes" << std::endl;
}

void bigTest() {
    std::vector<int> seq0(2000, 0);
    std::vector<int> seq1(2000, 0);
    for(size_t i = 0; i < 2000; ++i) {
        seq0[i] = i;
        seq1[i] = i;
    }
    std::cout << "Big test...";
    double time =
        sizeOfLcsIs(seq0.begin(), seq0.end(), seq1.begin(), seq1.end(), 2000);
    std::cout << " " << time << " secondes" << std::endl;
}

std::vector<int> randomVector(
    int maxValue, int minValue, size_t maxSize) {

    size_t size = rand() % maxSize + 1;
    std::vector<int> res(size, 0);

    for(int i = 0; i < size; ++i) {
        res[i] = rand() % (maxValue - minValue) + minValue;
    }

    return res;
}

//std::vector<size_t> getLcsIndex(
//    const std::vector<int> & seq0,
//    const std::vector<int> & seq1,
//    const std::vector<int> * indx) {
//
//    if (isCS(seq0, seq1, indx)) {
//        return indx;
//    }
//
//    std::vector<size_t> LCSindx;
//
//    for(size_t i = indx.size(); i > 0; --i) {
//        std::vector<size_t> tindx(indx);
//        tindx.erase(tindx.begin() + i - 1);
//
//        std::vector<size_t> ans = getLcsIndex(seq0, seq1, tindx);
//        if (ans.size() > LCSindx.size()) {
//            LCSindx = ans;
//        }
//    }
//
//    return LCSindx;
//}

void testSuite() {
    emptyTest();
    trivialTest(1, 2, 0, 1);
    trivialTest(1, 1, 1, 2);
    simpleTest();
    bigTest();
}

int main() {
    testSuite();

    return 0;
}

