#include <iostream>

constexpr bool is_prime(int number, int div) {
    return div == 1 ? true : number % div && is_prime(number, div - 1);
}

constexpr bool is_prime(int number) {
    return number <= 1 ? false : is_prime(number, number - 1);
}

int main() {
    static_assert(is_prime(29), "is not prime");
    // static_assert(is_prime(30), "is not prime");
    return 0;
}
