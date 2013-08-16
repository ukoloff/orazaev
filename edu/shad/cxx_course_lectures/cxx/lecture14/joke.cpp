#include <iostream>
#include <algorithm>
#include <cstdlib>


std::string randPermutation(std::string word) {
    if (word.size() < 4) {
        return word;
    }

    std::string::iterator begin = word.begin();
    std::string::iterator end = word.end();

    if (word.size() > 12) {
        begin += 3;
        end -= 3;
    } else if (word.size() > 8) {
        begin += 2;
        end -= 2;
    } else {
        begin += 1;
        end -= 1;
    }

    for (size_t i = 0; i < rand(); ++i) {
        std::next_permutation(begin, end);
    }

    return word;
}



int main() {
    std::srand(360);

    std::string word;
    while (std::cin >> word) {
        std::string joke = randPermutation(word);

        std::sort(word.begin(), word.end());
        std::cout << joke << " compare result = " << (joke == word) << std::endl;
    }

    return 0;
}
