#include<iostream>
#include<string>

bool check_polyndrom(std::string s) {
    int start = 0;
    int end = s.size() - 1;
    int polyndrom = 1;
    
    while(start < end) {
        if (s[start] != s[end]) {
            polyndrom = 0;
            break;
        }
        ++start;
        --end;
    }
    
    return polyndrom;
}

int main() {
    std::string stroka;
    
    while(getline(std::cin, stroka)) {
        if (check_polyndrom(stroka))
            std::cout << "It is a polyndrom!" << std::endl;
        else
            std::cout << "It is not a polyndrom!" << std::endl;
    }
    
    return 0;
}
