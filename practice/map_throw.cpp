#include <map>
#include <iostream>
#include <string>

int main() {
    std::map<int, std::string> a;
    std::cout << a[23] << std::endl;
    std::cout << a[25] << std::endl;
    std::cout << a[33] << std::endl;
    std::cout << a[13] << std::endl;

    std::map<std::string, char> b;
    std::cout << (int) b["asdf"] << std::endl;

    return 0;
}
