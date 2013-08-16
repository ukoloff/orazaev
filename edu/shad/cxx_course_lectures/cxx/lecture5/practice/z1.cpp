#include<iostream>
#include<vector>
#include<pair>
#include<map>
#include<string>
#include<algorithm>

void makeIndex(
    const std::vector<std::string> & lines,
    std::map<std::string, std::vector<std::pair<int, int> > > * const result) {
    
    size_t lineNumber = 0;
    for(std::vector<std::string>::iterator it = lines.begin();
        it != lines.end(); ++it) {

        size_t wordNumber = 0;

        std::string::iterator wordBegin = it->begin();

        // skip spaces.
        while(wordBegin != it->end() && *wordBegin == ' ') {
            ++wordBegin;
        }

        for(; wordBegin != it->end();
            wordBegin = std::find(wordBegin, it->end(), ' ')) {
            
            std::string::iterator wordEnd = std::find(wordBegin, it->end(), ' ');
            std::string word(wordBegin, wordEnd);

            (*result)[word].push_back(pair<int, int>(lineNumber, wordBegin - it->begin() ) );

            wordBegin = wordEnd;

            // skip spaces.
            while(wordBegin != it->end() && *wordBegin == ' ') {
                ++wordBegin;
            }
            ++wordNumber;
        }

        ++lineNumber;
    }
}

int main() {

    return 0;
}
