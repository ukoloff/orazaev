#include<iostream>
#include<map>
#include<string>

int main() {
    std::map<std::string, int> words_count;
    std::string word;

    while(std::cin.good()) {
        std::cin >> word;
        ++words_count[word];
    }

    for(std::map<std::string, int>::const_iterator it = words_count.begin();
        it != words_count.end(); ++it)
        std::cout << it->first << " " << it->second << std::endl;
}
