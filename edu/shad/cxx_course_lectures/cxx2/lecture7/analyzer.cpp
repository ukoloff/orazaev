#include "dict.h"
#include <iostream>

static TDictionary foo(std::vector<std::string>({"a", "b", "c"}));
static TDictionary bar(std::vector<std::string>({"1", "2"}));

int main() {
    for (auto dict = GetDictionaries()->begin();
         dict != GetDictionaries()->end();
         ++dict)
    {
        std::cout << (**dict).GetWords()->size() << std::endl;
        for (auto word = (*dict)->GetWords()->begin();
             word != (*dict)->GetWords()->end();
             ++word)
        {
            std::cout << *word << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
