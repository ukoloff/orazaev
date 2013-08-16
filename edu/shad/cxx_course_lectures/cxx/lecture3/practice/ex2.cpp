#include<iostream>

int atoi(const char * str) {
    int ans = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        ++str;
    }
    if (*str == '+') 
        ++str;

    while(*str != 0) {
        ans *= 10;
        ans += (*str++ - '0');
    }

    return ans * sign;
}

int main() {
    std::cout << atoi("-1234") << std::endl;
    std::cout << atoi("1234") << std::endl;
    std::cout << atoi("+1234") << std::endl;
    std::cout << atoi("0") << std::endl;
    std::cout << atoi("") << std::endl;
    
}
