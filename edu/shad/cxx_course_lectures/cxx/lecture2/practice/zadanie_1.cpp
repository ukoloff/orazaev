#include<iostream>

unsigned power(unsigned a, unsigned n) {
    if (n == 0) return 1;

    if (n % 2 == 0) {
        unsigned temp = power(a, n/2);
        return temp * temp;
    } else {
        unsigned temp = power(a, (n-1)/2);
        return temp * temp * 2;
    }
    
}

int main() {
    unsigned a, n;
    std::cin >> a;
    std::cin >> n;

    std::cout << a << "^" << n << " = " << power(a, n) << std::endl;

    return 0;
}
