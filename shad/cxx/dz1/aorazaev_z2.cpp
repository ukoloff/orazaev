/*
 *      C++ homework. Problem 2.
 * 
 *      Due date: 04.09.12
 *
 *      Author: Aman Orazaev
 */
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<stdexcept>



int gcdex(int a, int b, int & x, int & y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }

    int x1, y1;
    int d = gcdex(b%a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
   return d;
}

// Mathematical description:
//     inverse(a, n) * a = 1 (mod n)
//
// If there are no inverse,
//     return 0
int inverse(int a, int n) {
    int x, y;
    int d = gcdex(a, n, x, y);
    
    if (d != 1 || a == 0 || n <= 0) {
        return 0;
    } else {
        int ans = y > 0 ? n - std::abs(x) : x;
        return a < 0 ? -ans : ans;
    }
}



void stressTest(int N) {
    for(int i = 1; i <= N; ++i) {
        std::cout << "Test number: " << i << std::endl;

        // There are no inverse if n <= 0,
        // becouse x % n always will be < 0, and
        // never be 1.
        int n = rand() % 1000000 + 1;
        int a = rand() % 2000 - 1000;
        int ans = inverse(a, n);

        // There are no inverse if a == 0, n == 1 
        // and if gcd(a, n) != 1
        int x, y;
        if (a == 0 || n == 1 || gcdex(a, n, x, y) != 1) 
            continue;

        if ((ans*a) % n != 1) {
            std::cout << "Inverse error!" << std::endl;
            std::cout << "a       = " << a << std::endl
                      << "n       = " << n << std::endl
                      << "inverse = " << ans << std::endl
                      << "ans*a mod n = " << (ans * a) % n << std::endl;
            throw std::runtime_error("Test filed!");
        } 
    }
}

int main(int argc, char* argv[]) {
    std::srand(360);
    stressTest(1000000);
    return 0;
}

