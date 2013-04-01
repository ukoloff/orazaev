/**
 *  Algorithms and data structures (part 2). Home work 1.
 *  Problem 1: prefix function.
 *
 *  Copyright 2013 Aman Orazaev
 */
#include <string>
#include <iostream>
#include <vector>


template <typename T>
void PrintElements(T begin, T end, std::ostream* out = &std::cout) {
    while (begin != end) {
        *out << *begin++ << " ";
    }
    *out << "\n";
}



std::vector<size_t> GetPrefixFunction(const std::string& string) {
    std::vector<size_t> prefixFunction(string.size(), 0);

    for (size_t index = 1; index < string.size(); ++index) {
        size_t prefixSize = prefixFunction[index - 1];
        while (prefixSize != 0 && string[index] != string[prefixSize]) {
            prefixSize = prefixFunction[prefixSize - 1];
        }

        if (string[index] == string[prefixSize]) {
            prefixFunction[index] = prefixSize + 1;
        }
    }

    return prefixFunction;
}




int main() {
    std::string input;
    std::cin >> input;

    std::vector<size_t> result = GetPrefixFunction(input);
    PrintElements(result.begin(), result.end());

    return 0;
}
