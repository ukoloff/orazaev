#include<iostream>

int strlen(char const * s) {
    int len = 0;
    while(*s++) ++len;
    return len;
}

int main() {
    const char * s = "Hello!";
    //char * s = "Hello!"; //Не компилируется.

    std::cout << strlen(s) << std::endl;
}
