#include<iostream>
#include<vector>
#include<stdexcept>

size_t max(size_t x, size_t y) {
    return x > y ? x : y;
}

/* Larges Common Subsequence size */
template <typename A, typename B>
size_t sizeOfLCS(
    const A & begin0, const A & end0, 
    const B & begin1, const B & end1) {

    seqSize0 = end0 - begin0;
    seqSize1 = enc1 - begin1;
    
    std::vector<std::vector<size_t> > L = 
        std::vector(seqSize0 + 1, std::vector(seqSize1 + 1, 0));
    
    for(size_t i0 = 0; i0 < seqSize0; ++i0) {
        for (size_t i1 = 0; i1 < seqSize1; ++i1) {
            if (*(begin1 + i0) == *(begin1 + i1)) {
                L[i0 + 1][i1 + 1] = L[i0][i1] + 1;
            } else {
                L[i0 + 1][i1 + 1] =
                    max(L[i0 + 1][i1], L[i0][i1 + 1]);
            }
        }
    }
    
    return L[seqSize0][seqSize1];
}




/* Tests for sizeOfLCS */
template <typename A>
void printElements(
    const A & begin, const B & end, std::string space = " ") {

    for(A it = begin; it != end; ++it) {
        std::cout << *it << space;
    }

    std::cout << std::endl;
}

template <typename A, typename B>
void sizeOfLcsIs(
    const A & begin0, const A & end0, 
    const B & begin1, const B & end1, 
    size_t size) {
    
    size_t result = sizeOfLCS(begin0, end0, begin1, end1);
    if (result != size) {
        std::cout << "Sequence0: ";
        printElements(begin0, end0); std::cout << std::endl;
        std::cout << "Sequence1: ";
        printElements(begin1, end1); std::cout << std::endl;
        
        std:: cout << "Result of sizeOfLCS() = " << result;
        std:: cout << "Expected result       = " << size;
        throw std::runtime_error("TEST FAILED!")
    }
}


int main() {

    return 0;
}
