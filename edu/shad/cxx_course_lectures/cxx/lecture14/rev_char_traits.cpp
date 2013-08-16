#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>

struct rev_char_traits: public std::char_traits<char> {
    static int compare(const char* s1, const char* s2, size_t n) {
        for(size_t i = 0; i < n; ++i) {
            char first = *(s1 + strlen(s1) - i - 1);
            char second = *(s2 + strlen(s2) - i - 1);

            if (!eq(first, second)) {
                return lt(first, second) ? -1 : 1;
            }
        }
        return 0;
    }
};

typedef std::basic_string<char, rev_char_traits> MyString;


std::ostream& operator<< (std::ostream& out, const MyString& s) {
    std::cout << s.c_str() << std::endl;
}

template <typename T>
void printElements(T begin, T end) {
    while (begin != end) {
        std::cout << *begin++;
    }

    std::cout << std::endl;
}

int main() {
    std::vector<MyString> v;

    v.push_back(MyString("Payedb"));
    v.push_back(MyString("Pulledc"));
    v.push_back(MyString("Useda"));
    v.push_back(MyString("Played"));
    v.push_back(MyString("Aaaaaa"));

    printElements(v.begin(), v.end());

    std::sort(v.begin(), v.end());
    printElements(v.begin(), v.end());


    return 0;
}

