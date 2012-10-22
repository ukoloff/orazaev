/*
 *  Copyright (c) 2012 Aman Orazaev
 *
 *  Homework 2, Problem 1
 *
 *  Longest common sequence. (LCS)
 *
 */
#include<time.h>
#include<cstdlib>

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<stdexcept>

template <typename A>
void printElements(A begin, A end, std::string space = " ");



/* Larges Common Subsequence size */
template <typename A, typename B>
size_t sizeOfLCS(A beginFirst, A endFirst, B beginSecond, B endSecond) {
    size_t firstSeqSize = endFirst - beginFirst;
    size_t secondSeqSize = endSecond - beginSecond;

    std::vector<size_t> newL[2] = {
        std::vector<size_t>(secondSeqSize + 1, 0),
        std::vector<size_t>(secondSeqSize + 1, 0)
    };
    int line = 0;

    for (size_t i = 0; i < firstSeqSize; ++i) {
        for (size_t j = 0; j < secondSeqSize; ++j) {
            if (*(beginFirst + i) == *(beginSecond + j)) {
                newL[line][j + 1] = newL[(line + 1) % 2][j] + 1;
            } else {
                newL[line][j + 1] =
                    std::max(newL[(line + 1) % 2][j + 1], newL[line][j]);
            }
        }
        line = (line + 1) % 2;
    }

    return newL[(line + 1) % 2][secondSeqSize];
}




/* Tests for sizeOfLCS */
template <typename A>
void printElements(A begin, A end, std::string space = " ") {
    while (begin != end) {
        std::cout << *begin++ << space;
    }

    std::cout << std::endl;
}



template <typename A, typename B>
double sizeOfLcsIs(A beginFirst, A endFirst
    , B beginSecond, B endSecond, size_t size) {
    clock_t start = clock();
    size_t result = sizeOfLCS(beginFirst, endFirst, beginSecond, endSecond);
    clock_t end = clock();
    if (result != size) {
        std::cout << std::endl << "=============================="
            << std::endl;
        std::cout << "Sequence0: ";
        printElements(beginFirst, endFirst);
        std::cout << "Sequence1: ";
        printElements(beginSecond, endSecond);

        std:: cout << "Result of sizeOfLCS() = "
            << result << std::endl;
        std:: cout << "Expected result       = "
            << size << std::endl;
        throw std::runtime_error("TEST FAILED!");
    }
    return static_cast<double> (end - start) / CLOCKS_PER_SEC;
}



void simpleTest() {
    int firstSeq[5] = {1, 2, 3, 4, 5};
    int secondSeq[5] = {1, 3, 5, 7, 9};

    std::cout << "Simple test... ";
    double time =
        sizeOfLcsIs(firstSeq, firstSeq + 5, secondSeq, secondSeq + 5, 3);
    std::cout << " " << time << " secondes" << std::endl;
}



void emptyTest() {
    std::vector<int> firstSeq;
    std::vector<int> secondSeq;

    std::cout << "Empty test...";
    double time =
        sizeOfLcsIs(firstSeq.begin(), firstSeq.end()
            , secondSeq.begin(), secondSeq.end(), 0);
    std::cout << " " << time << " secondes" << std::endl;
}



void trivialTest(int firstSeqElem, int secondSeqElem
    , size_t expect, int testNo) {
    std::vector<int> firstSeq(1, firstSeqElem);
    std::vector<int> secondSeq(1, secondSeqElem);

    std::cout << "Trivial test" << testNo << "...";
    double time =
        sizeOfLcsIs(firstSeq.begin(), firstSeq.end()
            , secondSeq.begin(), secondSeq.end(), expect);
    std::cout << " " << time << " secondes" << std::endl;
}



void bigTest() {
    std::vector<int> firstSeq(2000, 0);
    std::vector<int> secondSeq(2000, 0);
    for (size_t i = 0; i < 2000; ++i) {
        firstSeq[i] = i;
        secondSeq[i] = i;
    }
    std::cout << "Big test...";
    double time =
        sizeOfLcsIs(firstSeq.begin(), firstSeq.end()
            , secondSeq.begin(), secondSeq.end(), 2000);
    std::cout << " " << time << " secondes" << std::endl;
}



std::vector<int> randomVector(int maxValue,
    int minValue, size_t maxSize) {

    size_t size = rand() % maxSize + 1;
    std::vector<int> res(size, 0);

    for (int i = 0; i < size; ++i) {
        res[i] = rand() % (maxValue - minValue) + minValue;
    }

    return res;
}



bool isCS(const std::vector<int> & firstSeq,
    const std::vector<int> & secondSeq,
    const std::vector<size_t> & indx) {

    size_t size = 0;
    for (std::vector<int>::const_iterator it = secondSeq.begin();
        it != secondSeq.end() && size < indx.size(); ++it) {
        if (*it == firstSeq[indx[size]]) {
            ++size;
        }
    }

    if (size == indx.size())
        return true;
    return false;
}



std::vector<size_t> getLcsIndex(
    const std::vector<int> & firstSeq,
    const std::vector<int> & secondSeq,
    const std::vector<size_t> & indx) {

    if (isCS(firstSeq, secondSeq, indx)) {
        return indx;
    }

    std::vector<size_t> LCSindx;

    for (size_t i = indx.size(); i > 0; --i) {
        std::vector<size_t> tindx(indx);
        tindx.erase(tindx.begin() + i - 1);

        std::vector<size_t> ans = getLcsIndex(firstSeq, secondSeq, tindx);
        if (ans.size() > LCSindx.size()) {
            LCSindx = ans;
        }
    }

    return LCSindx;
}



// Very slow implementation
size_t trivialSizeOfLCS(const std::vector<int> firstSeq,
    const std::vector<int> secondSeq) {

    std::vector<size_t> indx(firstSeq.size(), 0);

    for (size_t i = 1; i < indx.size(); ++i) {
        indx[i] = i;
    }

    return getLcsIndex(firstSeq, secondSeq, indx).size();
}



void stressTest(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        std::vector<int> firstSeq(randomVector(7, 0, 10));
        std::vector<int> secondSeq(randomVector(7, 0, 10));

        std::cout << "Stress test" << i << "...";
        size_t expected = trivialSizeOfLCS(firstSeq, secondSeq);
        double time =
            sizeOfLcsIs(firstSeq.begin(), firstSeq.end(),
                        secondSeq.begin(), secondSeq.end(), expected);
        std::cout << " " << time << " secondes" << std::endl;
    }
}



void testSuite() {
    std::srand(360);
    emptyTest();
    trivialTest(1, 2, 0, 1);
    trivialTest(1, 1, 1, 2);
    simpleTest();
    bigTest();
    stressTest(100);
}


std::vector<int> readSequenceFromInput() {
    size_t size;
    std::cin >> size;

    std::vector<int> seq;
    for (int i = 0; i < size; ++i) {
        int elem;
        std::cin >> elem;
        seq.push_back(elem);
    }

    return seq;
}


static void solveProblem() {
    std::vector<int> firstSeq =
        readSequenceFromInput();
    std::vector<int> secondSeq =
        readSequenceFromInput();

    std::cout <<
        sizeOfLCS(firstSeq.begin(), firstSeq.end()
            , secondSeq.begin(), secondSeq.end());
}



int main() {
    // testSuite();
    solveProblem();

    return 0;
}

