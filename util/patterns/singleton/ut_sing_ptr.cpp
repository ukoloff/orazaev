#include "TSingleton.h"
#include <iostream>
#include <string>

void ut_func(std::string* x) {
    *x = "aavdonkin@";
}

void ut_func1(std::string &x) {
    x = "quick@";
}

int main () {
    TSingleton<std::string *> x;

    x = new std::string("abcd");

    std::cout << *x << std::endl;

    delete x;


    TSingleton<std::string> foo;
    foo = "qwerty@";
    
    std::cout << foo << std::endl;

    ut_func(foo.inst());
    std::cout << foo << std::endl;

    ut_func1(*(foo.inst()));
    std::cout << foo << std::endl;
    
}
