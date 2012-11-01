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
void printElements(A begin, A end, std::string separator = " ");



/* Larges Common Subsequence size */
template <typename A, typename B>
size_t sizeOfLCS(A beginFirst, A endFirst, B beginSecond, B endSecond) {
    size_t firstSeqSize = endFirst - beginFirst;
    size_t secondSeqSize = endSecond - beginSecond;

    std::vector<std::vector<size_t> > LCSSizes;

    LCSSizes.push_back(std::vector<size_t>(secondSeqSize + 1, 0));
    LCSSizes.push_back(std::vector<size_t>(secondSeqSize + 1, 0));

    int current = 0;
    int next = 1;

    for (size_t subSeq0len = 0; subSeq0len < firstSeqSize; ++subSeq0len) {
        for (size_t subSeq1len = 0; subSeq1len < secondSeqSize; ++subSeq1len) {
            if (*(beginFirst + subSeq0len) == *(beginSecond + subSeq1len)) {
                LCSSizes[current][subSeq1len + 1] = LCSSizes[next][subSeq1len] + 1;
            } else {
                LCSSizes[current][subSeq1len + 1] =
                    std::max(LCSSizes[next][subSeq1len + 1], LCSSizes[current][subSeq1len]);
            }
        }

        current = next;
        next = (next + 1) % 2;
    }

    return LCSSizes[next][secondSeqSize];
}




/* Tests for sizeOfLCS */
template <typename A>
void printElements(A begin, A end, std::string separator = " ") {
    while (begin != end) {
        std::cout << *begin++ << separator;
    }

    std::cout << std::endl;
}



template <typename A>
double sizeOfLCSEqual(A beginFirst,
        A endFirst,
        A beginSecond,
        A endSecond,
        size_t size) {
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
        sizeOfLCSEqual(firstSeq, firstSeq + 5, secondSeq, secondSeq + 5, 3);
    std::cout << " " << time << " secondes" << std::endl;
}



void emptyTest() {
    std::vector<int> firstSeq;
    std::vector<int> secondSeq;

    std::cout << "Empty test...";
    double time =
        sizeOfLCSEqual(firstSeq.begin(), firstSeq.end()
            , secondSeq.begin(), secondSeq.end(), 0);
    std::cout << " " << time << " secondes" << std::endl;
}



void trivialTest(int firstSeqElem, int secondSeqElem
    , size_t expect, int testNo) {
    std::vector<int> firstSeq(1, firstSeqElem);
    std::vector<int> secondSeq(1, secondSeqElem);

    std::cout << "Trivial test" << testNo << "...";
    double time =
        sizeOfLCSEqual(firstSeq.begin(), firstSeq.end()
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
        sizeOfLCSEqual(firstSeq.begin(), firstSeq.end()
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
    const std::vector<size_t> & indexes) {

    size_t size = 0;
    for (std::vector<int>::const_iterator it = secondSeq.begin();
        it != secondSeq.end() && size < indexes.size(); ++it) {
        if (*it == firstSeq[indexes[size]]) {
            ++size;
        }
    }

    if (size == indexes.size())
        return true;
    return false;
}



std::vector<size_t> getLcsIndex(
    const std::vector<int> & firstSeq,
    const std::vector<int> & secondSeq,
    const std::vector<size_t> & indexes) {

    if (isCS(firstSeq, secondSeq, indexes)) {
        return indexes;
    }

    std::vector<size_t> LCSindexes;

    for (size_t i = indexes.size(); i > 0; --i) {
        std::vector<size_t> tindexes(indexes);
        tindexes.erase(tindexes.begin() + i - 1);

        std::vector<size_t> ans = getLcsIndex(firstSeq, secondSeq, tindexes);
        if (ans.size() > LCSindexes.size()) {
            LCSindexes = ans;
        }
    }

    return LCSindexes;
}



// Very slow implementation
size_t trivialSizeOfLCS(const std::vector<int> firstSeq,
    const std::vector<int> secondSeq) {

    std::vector<size_t> indexes(firstSeq.size(), 0);

    for (size_t i = 1; i < indexes.size(); ++i) {
        indexes[i] = i;
    }

    return getLcsIndex(firstSeq, secondSeq, indexes).size();
}



void stressTest(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        std::vector<int> firstSeq(randomVector(7, 0, 10));
        std::vector<int> secondSeq(randomVector(7, 0, 10));

        std::cout << "Stress test" << i << "...";
        size_t expected = trivialSizeOfLCS(firstSeq, secondSeq);
        double time =
            sizeOfLCSEqual(firstSeq.begin(), firstSeq.end(),
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

