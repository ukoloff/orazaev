#ifndef _SPLIT_STRING_
#define _SPLIT_STRING_

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

template <typename A>
void printElements(A begin, A end) {
    if (begin == end)
        return;

    for (A i = begin; i != end; ++i) {
        std::cout << *i << "|";
    }

    std::cout << std::endl;
}


template <typename Inp, typename Out>
Out Split(const std::string & str, Inp firstDelim, Inp lastDelim,
          Out result) {
    size_t searchPos = 0;

    if (std::find(firstDelim, lastDelim, "") != lastDelim) {
        firstDelim = lastDelim;
    }


    while (searchPos < str.size()) {
        std::string element;
        size_t delimSize = 0;

        if (firstDelim != lastDelim) {
            size_t curPos = static_cast<size_t>(-1);

            for (Inp it = firstDelim; it != lastDelim; ++it) {
                size_t founded = str.find(*it, searchPos);

                if (curPos > founded && founded != std::string::npos) {
                    curPos = founded;
                    delimSize = it->size();
                }
            }

            if (curPos == static_cast<size_t>(-1)) {
                break;
            }

            element = str.substr(searchPos, curPos - searchPos);
        } else {
            element = std::string(1, str[searchPos]);
        }

        *result++ = element;
        searchPos += element.size() + delimSize;
    }

    std::string element = str.substr(searchPos, str.size() - searchPos);

    if (element.size()) {
        *result++ = element;
    }

    return result;
}



void Split(const std::string & str,
           const std::string & delim,
           std::vector<std::string>* res) {
    size_t searchPos = 0;
    size_t curPos = 0;


    while (searchPos < str.size() &&
           (curPos = str.find(delim, searchPos)) != std::string::npos) {
        std::string element;
        if (!delim.empty()) {
            element = str.substr(searchPos, curPos - searchPos);
        } else {
            element = std::string(1, str[searchPos]);
        }

        res->push_back(element);
        searchPos += delim.size() + element.size();
    }

    std::string element = str.substr(searchPos, curPos - searchPos);

    if (element.size()) {
        res->push_back(element);
    }
}


void Split(const std::string & str, char delim,
           std::vector<std::string>* res) {
    Split(str, std::string(1, delim), res);
}


std::string Join(const std::vector<std::string> & strs,
                 const std::string & delim) {
    std::string result;
    for (std::vector<std::string>::const_iterator it = strs.begin();
         it != strs.end(); ++it) {
        result += *it;
        if (--strs.end() != it) {
            result += delim;
        }
    }

    return result;
}

#endif // _SPLIT_STRING_
