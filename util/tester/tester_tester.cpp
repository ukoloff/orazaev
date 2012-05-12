#include <iostream>
#include <string>
#include "tester.h"

int right(int x) {
    return x + 1;
}

int wrong(int x) {
    return x;
}

std::string chr(int x) {
    char c[2] = { x, 0 };
    return std::string(c);
}

int main() {
    Tester::ptest(1,1);
    std::cout << Tester::test(right(1), 2) << std::endl; 
    std::cout << Tester::test(right(2), 3) << std::endl; 
    std::cout << Tester::test(right(-1), 0) << std::endl; 
    std::cout << Tester::test(right(0), 1) << std::endl;

    std::cout << Tester::test(wrong(0), 1) << std::endl;

    TPair<int, std::string> p(23, "Michael Jordon");
    
    std::cout << p.First() << " " << p.Second() << std::endl;
    std::cout << (p.First() = 11) << std::endl;
    std::cout << p.First() << " " << p.Second() << std::endl;
    std::cout << p << std::endl;

    // ptt --- pair to test
    typedef TPair<int, int> ptt;
    
    ptt k[3] = {
        ptt(right(1), 2),
        ptt(right(2), 3),
        ptt(right(3), 4)
    };
    Tester::pntest(3, k);

    return 0;
}
