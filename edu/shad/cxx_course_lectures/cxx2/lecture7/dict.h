#ifndef __DICT_H__
#define __DICT_H__

#include <vector>
#include <string>
#include <memory>

class TDictionary;

std::vector<TDictionary*> * GetDictionaries();

class TDictionary {
public:
    TDictionary(const std::vector<std::string>& words)
        : words(words)
    {
        GetDictionaries()->push_back(this);
    }

    std::vector<std::string> * GetWords() {
        return &words;
    }

private:
    std::vector<std::string> words;
};

#endif // __DICT_H__
