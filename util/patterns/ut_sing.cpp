#include "TSingleton1.h"
#include <iostream>
#include <string>

void string_print(std::string s) {
    std::cout << s << std::endl;
}

int main() {
    TSingleton<int> si;
    std::cout << si << std::endl;

    TSingleton<int> si2(new int(10));
    
    std::cout << si << std::endl;
    

    TSingleton<std::string> ss(new std::string("stroka odna"));
        
    std::cout << ss.inst() << std::endl;

    string_print(ss);
    std::cout.operator<<(ss); std::cout << std::endl;
    //ss = std::string("Amazing!");
    std::cout << TSingleton<std::string>();
}
