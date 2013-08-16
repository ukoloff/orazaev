#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

#include "split_string.h"


const size_t MAX_LINE_SIZE = 256;

std::string getLine(std::istream &);



class TTezarus {
    typedef std::set<std::string> TTezarusData;
    typedef TTezarusData::iterator Iter;
    typedef std::map<Iter, std::vector<Iter> > TTezarusRef;

    TTezarusData data;
    TTezarusRef ref;

    void addToData(const std::string & d);

    public:
    TTezarus() { }

    void init(std::istream & in);

};

void TTezarus::init(std::istream & in) {
    while (in.good()) {
        std::string line = getLine(in);

        std::vector<std::string> words;
        Split(line, ' ', &words);

        if (!words.empty()) {
            std::pair<Iter, bool> key;
            key = data.insert(words[0]);

            for (std::vector<std::string>::const_iterator it = ++words.begin();
                 it != words.end(); ++it) {
                std::pair<Iter, bool> elem;
                elem = data.insert(*it);

                ref[key.first].push_back(elem.first);
            }
        }
    }
}


std::string getLine(std::istream & in) {
    char l[MAX_LINE_SIZE];
    in.getline(l, MAX_LINE_SIZE);

    return std::string(l);
}

int main() {
    TTezarus tez;
    tez.init(std::cin);

    return 0;
}
