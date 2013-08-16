#include<iostream>

int main() {

    int i = 10;

    int k_prefix = ++i;
    i = 10;
    int k_postfix = i++;
    
    std::cout << "prefix: " << k_prefix << std::endl;
    std::cout << "postfix: " << k_postfix << std::endl;

    return 0;
}
