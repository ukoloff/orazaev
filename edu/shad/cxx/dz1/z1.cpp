#include<iostream>
#include<cstdlib>

unsigned long f (unsigned int a, unsigned int n) {
    unsigned long r = 1;
    while (n > 0) {
        if (n % 2 == 1) {
            r *= a;
            n--;
        } else {
            r *= r;
            n /= 2;
        }
    }
    return r;
}

unsigned long f_right (unsigned int a, unsigned int n) {
    if (n == 0) 
        return 1;
    unsigned long r = a;
    unsigned x = 1;
    
    while (n > 1) {
        if (n % 2 == 1) {
            x *= r;
            n--;
        } else {
            r *= r;
            n /= 2;
        }
    }
    r *= x;
    return r;
}

int main(int argc, char* argv[]) {
    for(int i = 1; (i + 1) < argc; ++i) {
        unsigned a = static_cast<unsigned>(atoi(argv[i]));
        unsigned b = static_cast<unsigned>(atoi(argv[++i]));

        std::cout << a << "**" << b << " = ";
        std::cout << f(a, b) << std::endl;
    }
    return 0;
}

