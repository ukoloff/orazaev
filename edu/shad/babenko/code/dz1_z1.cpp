/*
 *  Homework 1, Problem 1
 *
 *  Longest alternating sequence. (LAS)
 * 
 *  Author: Orazaev Aman
 *
 *  TODO:
 *  [ ] fix LCA recovery bug. find better recovering.
 *  [ ] There are bug with choose previouse elem in LCA.
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

size_t comparePreviousIndex(const std::vector<size_t> * const P,
                            size_t j0, size_t j1, char c) {
    if (j1 == NA)
        return j0;
    if (j0 == NA)
        return j1;

    std::vector<size_t> indx0(1, j0);
    std::vector<size_t> indx1(1, j1);
    
    
    for(j0 = P[c][j0], j1 = P[c][j1]; j0 != NA;
        j0 = P[c][j0], j1 = P[c][j1]) {
        indx0.push_back(j0);
        indx1.push_back(j1);
        c = (c + 1) % 2;
    }
    
    size_t ans = j0;
    
    while(!indx0.empty()) {
        if (indx0.back() != indx1.back()) {
            ans = indx0.back() > indx1.back() ? j1 : j0;
            break;
        }
        indx0.pop_back();
        indx1.pop_back();
    }

    return ans;
}
            

std::vector<size_t> recoverIndexesFromLP(const std::vector<int> & seq,
                                       const std::vector<int> * const L, 
                                       const std::vector<size_t> * const P,
                                       size_t c) {
    std::vector<size_t> ansIndx;
    for(size_t e = std::max_element(L[c].begin(), L[c].end()) - L[c].begin();
        e < L[c].size(); ++e) {
        size_t tryE = e;
        size_t tryC = c;
        std::vector<size_t> tryIndx;

        while(tryE != NA) {
            tryIndx.push_back(tryE);

            tryE = P[tryC][tryE];
            tryC = (tryC + 1) % 2;
        }
        std::reverse(tryIndx.begin(), tryIndx.end());

        if (ansIndx.empty()) {
            ansIndx = tryIndx;
        } else if(compareAS(seq, tryIndx, ansIndx) > 0) {
            ansIndx = tryIndx;
        }
    }

    return ansIndx;
}

std::vector<int> LAS(const std::vector<int> & seq) {
    if (seq.empty())
        return std::vector<int>();

    std::vector<int> L[2] = { 
        std::vector<int>(seq.size(), 1),
        std::vector<int>(seq.size(), 1)
    };
    std::vector<size_t> P[2] = { 
        std::vector<size_t>(seq.size(), NA),
        std::vector<size_t>(seq.size(), NA)
    };

    for(int i = 0; i < seq.size(); ++i) {
        for(int j = 0; j < i; ++j) {
            if (seq[j] < seq[i] && L[GT][j] >= L[LT][i]) {
                L[LT][i] = L[GT][j] + 1;
                P[LT][i] = j;
                continue;
            }
            if (seq[j] > seq[i] && L[LT][j] >= L[GT][i]) {
                L[GT][i] = L[LT][j] + 1;
                P[GT][i] = j;
                continue;
            }

            if (seq[j] < seq[i] && L[GT][j] == (L[LT][i] - 1)) {
                printElements(seq.begin(), seq.end());
                printElements(L[GT].begin(), L[GT].end());
                printElements(P[GT].begin(), P[GT].end());
                std::cout << (int) LT << " " << j << " " << P[LT][i] << std::endl;
                P[LT][i] = comparePreviousIndex(P, j, P[LT][i], GT);
            }
            if (seq[j] > seq[i] && L[LT][j] == (L[LT][i] - 1)) {
                printElements(seq.begin(), seq.end());
                printElements(L[GT].begin(), L[GT].end());
                printElements(P[GT].begin(), P[GT].end());
                std::cout << (int) GT << " " << j << " " << P[GT][i] << std::endl;
                P[GT][i] = comparePreviousIndex(P, j, P[GT][i], LT);
            }
        }
    }

    std::vector<size_t> ansIndx0 = recoverIndexesFromLP(seq, L, P, LT);
    std::vector<size_t> ansIndx1 = recoverIndexesFromLP(seq, L, P, GT);

    std::vector<size_t> * ansIndx = 
        compareAS(seq, ansIndx0, ansIndx1) > 0 ? &ansIndx0 : &ansIndx1;
                                    
    std::vector<int> ans;
    for(std::vector<size_t>::const_iterator it = ansIndx->begin();
        it != ansIndx->end(); ++it)
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
    std::cout << "EMPTY SEQUENSE TEST ... [ START ]" << std::endl;

    std::vector<int> seq;
    std::vector<int> ans = LAS(seq);
    
    if (!ans.empty())
        throw std::runtime_error("TEST FAILED: empty sequence.");

    std::cout << "EMPTY SEQUENSE TEST ... [ OK ]" << std::endl;
}

void trivialSequenceTest() {
    std::cout << "TRIVIAL SEQUENS TEST ... [ START ]" << std::endl;

    std::vector<int> seq(20, 360);
    std::vector<int> rightAns(1, 360);

    std::vector<int> ans = LAS(seq);
    assertEqualVectors(ans, rightAns);

    std::cout << "TRIVIAL SEQUENS TEST ... [ OK ]" << std::endl;
}

void bigSequenceTest() {
    std::cout << "BIG SEQUENS TEST ... [ START ]" << std::endl;

    std::vector<int> seq;
    for(int i = 0; i < 1000; ++i) {
        seq.push_back(i % 2 == 0 ? NA - 1 : 1 - NA);
    }

    std::vector<int> ans = LAS(seq);
    assertEqualVectors(ans, seq);

    std::cout << "BIG SEQUENS TEST ... [ OK ]" << std::endl;
}

void simpleSequenceTest() {
    std::cout << "SIMPLE SEQUENS TEST ... [ START ]" << std::endl;

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

    std::cout << "SIMPLE SEQUENS TEST ... [ START ]" << std::endl;
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
    simpleSequenceTest();
    //LAS_TestCase();
    
    return 0;
}
