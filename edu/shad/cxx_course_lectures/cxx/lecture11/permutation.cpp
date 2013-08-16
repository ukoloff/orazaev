#include <string>
#include <algorithm>
#include <iostream>

int main() {
    std::string word;

    std::cin >> word;

    std::sort(word.begin(), word.end());
    do {
        std::cout << word << std::endl;
    } while (std::next_permutation(word.begin(), word.end()));
}
