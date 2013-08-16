#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include <cctype>

#include "split_string.h"

const size_t MAX_LINE_SIZE = 256;



void strVectortoLower(std::vector<std::string> * const vec) {
    for (std::vector<std::string>::iterator it = vec->begin();
         it != vec->end(); ++it) {
        for (std::string::iterator ch = it->begin();ch != it->end(); ++ch) {
            if (*ch <= 'Z' && *ch >= 'A') {
                *ch -= 'Z' - 'z';
            }
        }
    }
}

std::string getLine(std::istream & in) {
    char l[MAX_LINE_SIZE];
    in.getline(l, MAX_LINE_SIZE);

    return std::string(l);
}

bool isAlphaString(const std::string & str) {
    if (str.empty())
        return false;

    for (std::string::const_iterator it = str.begin();
         it != str.end(); ++it) {
        if (*it > 'z' || *it < 'a') {
            return false;
        }
    }

    return true;
}

void createPointer(
        std::istream & in,
        std::map<std::string, std::set<size_t> > * const pointer) {
    size_t lineNo = 0;

    while (in.good()) {
        std::string line = getLine(in);
        lineNo += 1;

        std::vector<std::string> words;
        Split(line, ' ', &words);

        strVectortoLower(&words);

        for (std::vector<std::string>::const_iterator word = words.begin();
             word != words.end(); ++word) {
            if (isAlphaString(*word)) {
                (*pointer)[*word].insert(lineNo);
            }
        }
    }
}


void printPointer(const std::map<std::string, std::set<size_t> > & p) {
    for (std::map<std::string, std::set<size_t> >::const_iterator it = p.begin();
         it != p.end(); ++it) {
        std::cout << it->first << ": ";

        for (std::set<size_t>::const_iterator line = it->second.begin();
             line != it->second.end(); ++line) {
            std::cout << *line << " ";
        }

        std::cout << std::endl;
    }
}


int main() {
    std::map<std::string, std::set<size_t> > p;

    createPointer(std::cin, &p);
    printPointer(p);

    return 0;
}
