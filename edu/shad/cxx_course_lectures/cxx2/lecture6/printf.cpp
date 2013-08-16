#include <iostream>
#include <string>
#include <stdexcept>
#include <string.h>


void Printf(const char * command_str) {
    for (size_t i = 0; i < strlen(command_str); ++i) {
        if (command_str[i] == '%' && command_str[i + 1] != '%') {
            throw std::runtime_error("Incorrect number of arguments.");
        }

        if (command_str[i] == '%') {
            ++i;
            std::cout << '%';
            continue;
        }

        std::cout << command_str[i];
    }
}

template <typename T, typename ... Type>
void Printf(const char * command_str, T arg, Type ... args) {
    size_t i = 0;
    for (; i < strlen(command_str); ++i) {
        if (command_str[i] == '%' && command_str[i + 1] != '%') {
            std::cout << arg;
            break;
        }

        if (command_str[i] == '%') {
            ++i;
            std::cout << '%';
            continue;
        }

        std::cout << command_str[i];
    }

    if (i == strlen(command_str)) {
        throw std::runtime_error("Incorrect number of arguments.");
    }

    Printf(command_str + i + 2, args ...);
}

int main() {
    Printf("Hello, world! %d Michael %s\n", 23, "Jordan");
    Printf("Hello, world! %% Michael %s\n", "Jordan");

    return 0;
}
