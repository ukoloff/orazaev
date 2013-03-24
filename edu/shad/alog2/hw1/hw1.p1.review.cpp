/**
 *  Algorithms and data structures (part 2). Home work 1.
 *  Problem 1: prefix function.
 *
 *  Copyright 2013 Aman Orazaev
 */

#include <cstdlib>

#include <string>
#include <iostream>
#include <vector>


template <typename T>
void PrintVector(T begin, T end, std::ostream* out = &std::cout) {
    while (begin != end) {
        *out << *begin++ << (begin + 1 == end ? "" : " ");
    }
    std::cout << "\n";
}



std::vector<size_t> GetPrefixFunction(const std::string& string) {
    std::vector<size_t> prefixFunction(string.size(), 0);

    for (size_t curent = 1; curent < string.size(); ++curent) {
        size_t prefixEnd = prefixFunction[curent - 1];
        while (prefixEnd != 0 && string[curent] != string[prefixEnd]) {
            prefixEnd = prefixFunction[prefixEnd - 1];
        }

        if (string[curent] == string[prefixEnd]) {
            prefixFunction[curent] = prefixEnd + 1;
        }
    }

    return prefixFunction;
}


// #include "hw1.p1.testing.h"


int main() {
    std::string input;
    std::cin >> input;

    std::vector<size_t> pref = GetPrefixFunction(input);
    PrintVector(pref.begin(), pref.end());

    // return RunTests();
    return 0;
}
