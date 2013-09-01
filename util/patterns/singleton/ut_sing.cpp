#include "TSingleton.h"
#include <iostream>
#include <string>

void string_print(std::string s) {
    std::cout << s << std::endl;
}

void ut_string() {
    TSingleton<std::string> ss(new std::string("stroka odna"));
    std::cout << ss.inst() << std::endl;

    ss = std::string("Amazing!");
    std::cout << ss << std::endl;
    std::cout << TSingleton<std::string>() << std::endl;
    
    TSingleton<std::string> ss1;
    ss1 = "Some new text";
    std::cout << ss << std::endl;
    std::cout << ss1 << std::endl;

    TSingleton<std::string> ss2(new std::string("Another new text!"));
    std::cout << ss << " " << ss1 << " " << ss2 << std::endl;
    while(1) {
    TSingleton<std::string> ss3(new std::string("Templary text"));
    std::cout << ss3 << " count " << ss3.getRefCount() << std::endl;
    break;
    }
    std::cout << ss << " " << ss1 << " " << ss2 << " count " << ss.getRefCount() << std::endl;
    
    std::cout << TSingleton<std::string>().getRefCount() << " " << TSingleton<std::string>().getRefCount() << std::endl;
    std::cout << ss.getRefCount() << std::endl;

    std::cout << TSingleton<char>().getRefCount() << std::endl;
    std::cout << TSingleton<char>().getRefCount() << std::endl;

    string_print(TSingleton<std::string>());
}

int main() {
    TSingleton<int> si;
    std::cout << si << std::endl;

    TSingleton<int> si2(new int(10));
    si++;
    
    std::cout << si << std::endl;
    std::cout << si++ << std::endl;
    
    ut_string();
    std::cout << TSingleton<std::string>() << std::endl;

    for( TSingleton<int> x; x > 0; x = x - 1) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << si << " " << si2 << std::endl;

    TSingleton<int>(new int(4));
    std::cout << si << std::endl;
    si -= 4;
    
    if (si)
        std::cout << "It's non zero!" << std::endl;
    if (!si)
        std::cout << "It's zero!" << std::endl;

    si |= 4;

    if (si)
        std::cout << "It's non zero!" << std::endl;
    if (!si)
        std::cout << "It's zero!" << std::endl;
   
    std::cout << si << std::endl;
}
