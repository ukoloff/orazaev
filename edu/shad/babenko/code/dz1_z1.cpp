/*
 *  Homework 1, Problem 1
 *
 *  Longest alternating sequence. (LAS)
 * 
 *  Author: Orazaev Aman
 *
 *  TODO:
 *  [ ] fix LCA recovery bug. find better recovering.
 */
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<stdexcept>
#include<cstdlib>

// "LT" and "GT" is bash/perl-like "<" and ">"
const size_t LT = 0;
const size_t GT = 1;

// Input elements less than 10^9
// So we could let not available
// element to be 10^9 + 1
const int NA = 1000000001;



template <typename T>
void printElements(T begin, T end, std::string space = " ") {
    while(begin != end) {
        std::cout << *begin++ << space;
    }
    std::cout << std::endl;
}

int compareAS(const std::vector<int> & seq,
              const std::vector<size_t> & indx0,
              const std::vector<size_t> & indx1) {
    if (indx0.size() != indx1.size()) {
        return indx0.size() > indx1.size() ? 1 : -1;
    }

    int ans = 0;
    for(size_t i = 0; i < indx0.size(); ++i) {
        if (indx0[i] != indx1[i]) {
            ans = indx0[i] < indx1[i] ? 1 : -1;
            break;
        }
    }

    return ans;
}

size_t getGreaterLAS(const std::vector<int> * L) {
    size_t ans = LT;
    for(size_t i = L[LT].size(); i > 0; --i) {
        if (L[LT][i - 1] == L[GT][i - 1])
            continue;

        ans = L[LT][i - 1] > L[GT][i - 1] ? LT : GT;
        break;
    }

    return ans;
}


std::vector<int> LAS(const std::vector<int> & seq) {
    if (seq.empty())
        return std::vector<int>();

    std::vector<int> L[2] = { 
        std::vector<int>(seq.size(), 1),
        std::vector<int>(seq.size(), 1)
    };
    std::vector<int> P[2] = { 
        std::vector<int>(seq.size(), NA),
        std::vector<int>(seq.size(), NA)
    };

    for(int i = 0; i < seq.size(); ++i) {
        for(int j = 0; j < i; ++j) {
            if (seq[j] < seq[i] && L[GT][j] >= L[LT][i]) {
                L[LT][i] = L[GT][j] + 1;
                P[LT][i] = j;
            }
            if (seq[j] > seq[i] && L[LT][j] >= L[GT][i]) {
                L[GT][i] = L[LT][j] + 1;
                P[GT][i] = j;
            }
        }
    }

    size_t c = getGreaterLAS(L);
    size_t e = std::max_element(L[c].begin(), L[c].end()) - L[c].begin();
    std::cout << e << std::endl;
    
    std::vector<size_t> ansIndx;

    for(; e < seq.size(); ++e) {
        size_t tryE = e;
        size_t tryC = c;
        std::vector<size_t> tryIndx;

        while(tryE != NA) {
            tryIndx.push_back(tryE);

            tryE = P[tryC][tryE];
            tryC = (tryC + 1) % 2;
        }
        std::reverse(tryIndx.begin(), tryIndx.end());
        printElements(tryIndx.begin(), tryIndx.end());
        printElements(ansIndx.begin(), ansIndx.end());

        if (ansIndx.empty()) {
            ansIndx = tryIndx;
        } else if(compareAS(seq, tryIndx, ansIndx) > 0) {
            ansIndx = tryIndx;
        }
    }

    std::vector<int> ans;
    for(std::vector<size_t>::const_iterator it = ansIndx.begin();
        it != ansIndx.end(); ++it)
        ans.push_back(seq[*it]);

    return ans;
}




/* --------------- Testing -------------- */
template <typename T>
void assertEqualVectors(const std::vector<T> & v1, const std::vector<T> & v2) {
    if (v1 != v2) {
        std::cout << "Vector 1: ";
        printElements(v1.begin(), v1.end());
        std::cout << "Vector 2: ";
        printElements(v2.begin(), v2.end());

        throw std::runtime_error("TEST FAILED: assertEqualVectors.");
    }
}

void emptySequenceTest() {
    std::vector<int> seq;
    std::vector<int> ans = LAS(seq);
    
    if (!ans.empty())
        throw std::runtime_error("TEST FAILED: empty sequence.");
}

void trivialSequenceTest() {
    std::vector<int> seq(20, 360);
    std::vector<int> rightAns(1, 360);

    std::vector<int> ans = LAS(seq);
    assertEqualVectors(ans, rightAns);
}

void bigSequenceTest() {
    std::vector<int> seq;
    for(int i = 0; i < 1000; ++i) {
        seq.push_back(i % 2 == 0 ? NA - 1 : 1 - NA);
    }

    std::vector<int> ans = LAS(seq);
    assertEqualVectors(ans, seq);
}

void simpleSequenceTest() {
    std::vector<int> seq;
    seq.push_back(7);
    seq.push_back(1);
    seq.push_back(3);
    seq.push_back(5);
    seq.push_back(4);

    std::vector<int> ans = LAS(seq);

    std::vector<int> rightAns;
    rightAns.push_back(7);
    rightAns.push_back(1);
    rightAns.push_back(5);
    rightAns.push_back(4);

    assertEqualVectors(ans, rightAns);
}

std::vector<int> randomVector(int maxValue, int minValue, 
                              size_t maxSize) {
    std::vector<int> res;

    size_t size = rand() % maxSize + 1;
    for(int i = 0; i < size; ++i) {
        res.push_back(rand() % (maxValue - minValue) + minValue);
    }

    return res;
}

bool isAS(const std::vector<int> seq, const std::vector<size_t> indx) {
    if (indx.size() < 3) {
        return true;
    }

    bool res = true;
    for(size_t i = 2; i < indx.size(); ++i) {
        if ((seq[indx[i]] > seq[indx[i - 1]] && 
            seq[indx[i - 1]] > seq[indx[i - 2]]) ||
            (seq[indx[i]] < seq[indx[i - 1]] &&
            seq[indx[i - 1]] < seq[indx[i - 2]])) {
            res = false;
            break;
        }
    }

    return res;
}


std::vector<size_t> getAS(const std::vector<int> & seq, 
                       const std::vector<size_t> & indx) {
    if (isAS(seq, indx)) {
        return indx;
    }

    std::vector<size_t> LASindx;

    for(size_t i = indx.size(); i > 0; --i) {
        std::vector<size_t> tindx(indx);
        tindx.erase(tindx.begin() + i - 1);
        
        std::vector<size_t> ans = getAS(seq, tindx);
        if (compareAS(seq, LASindx, ans) < 0) {
            LASindx = ans;
        }
    }
    
    return LASindx;
}

std::vector<int> trivialLAS(const std::vector<int> & seq) {
    std::vector<size_t> indxs;
    for(int i = 0; i < seq.size(); ++i)
        indxs.push_back(i);

    
    indxs = getAS(seq, indxs);

    std::vector<int> ans;
    for(std::vector<size_t>::const_iterator it = indxs.begin();
        it != indxs.end(); ++it)
        ans.push_back(seq[*it]);

    return ans;
}

void stressTest(size_t N) {
    for(size_t testNo = 1; testNo <= N; ++testNo) {
        std::vector<int> seq(randomVector(1000, -1000, 6));

        std::cout << "STRESS NO " << testNo << std::endl;
        std::cout << "Input: ";
        printElements(seq.begin(), seq.end());
        assertEqualVectors(LAS(seq), trivialLAS(seq));
        std::cout << std::endl;
    }
}

void LAS_TestCase() {
    std::srand(360);

    emptySequenceTest();
    trivialSequenceTest();
    bigSequenceTest();
    simpleSequenceTest();
    stressTest(1000);
}

int main() {
    LAS_TestCase();
    
    return 0;
}
