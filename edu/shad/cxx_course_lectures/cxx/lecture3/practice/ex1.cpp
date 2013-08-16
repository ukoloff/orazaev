#include<iostream>
#include<stdio.h>

int sign(int a) {
    if (a == 0) 
        return 0;
    return a > 0 ? 1 : -1;
}

int strlen(const char * str) {
    int ans = 0;
    while(*str++) 
        ++ans;
    return ans;
}

char * strcat(char * destination, const char * source) {
    char * ans = destination;
    int len = strlen(destination);
    while(*(destination + len) = *source++)
        ++destination;
    *(destination + len) = 0;
    
    return ans;
}

char * strrev(char * destination, const char * source) {
    char * ans = destination;
    int len = strlen(source);
    while(*destination++ = *(source + len - 1))
        --source;
    *destination = 0;

    return ans;
}

int strcmp(const char * str1, const char * str2) {
    int ans = 0;
    while ((ans = *str1 - *str2++) == 0 && *str1++ != 0);
        
    return ans;
}

int main() {
    std::cout << strcmp("abc", "abcd") << " = " << -1 << std::endl;
    std::cout << strcmp("abcde", "abc") << " = " << 1 << std::endl;
    std::cout << strcmp("bcde", "abc") << " = " << 1 << std::endl;
    std::cout << strcmp("abcd","abcd") << " = " << 0 << std::endl;
    std::cout << strcmp("asdf", "") << " = " << 1 << std::endl;
    
    char c[100] = "";
    strcat(c, "Hello, world!");
    printf("%s\n", c);
    strcat(c, "xxxaaabbb");
    printf("%s\n", c);

    std::cout << strlen("") << " = " << 0 << std::endl;
    std::cout << strlen("abc") << " = " << 3 << std::endl;
    std::cout << strlen(c) << std::endl;
    
    strrev(c, "1234512");
    printf("%s\n", c);

    strrev(c, "");
    printf("%s\n", c);
}
