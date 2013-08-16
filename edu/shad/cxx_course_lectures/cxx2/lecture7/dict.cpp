#include "dict.h"

std::vector<TDictionary*> * GetDictionaries() {
    static std::vector<TDictionary*> dicts;
    return &dicts;
}
