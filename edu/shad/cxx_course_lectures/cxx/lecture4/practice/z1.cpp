#include<iostream>
#include<cmath>
#include<unistd.h>

using std::abs;

const double EPSILON = 0.01;

double sign(double x) {
    return x > 0 ? 1 : (x == 0 ? 0 : -1);
}

double square(double x) {
    return x*x - 16;
}

double solve(double a, double b, double (*f)(double)) {
    double ans;
    if (a > b) {
        double t = a;
        a = b;
        b = t;
    }

    if (f(a)*f(b) > 0) {
        return 0;
    } else {
        while(abs(f(a + (b - a) / 2)) > EPSILON) {
            if (sign(f(a + (b - a) / 2)) != sign(f(a))) {
                b = a + (b - a) / 2;
            } else {
                a += (b - a) / 2;
            }
        }
    }
    return a + (b - a) / 2;
}

int main() {
    std::cout << solve(-4, 10, square) << std::endl;
    std::cout << solve(-3, 10, square) << std::endl;
    std::cout << solve(10, -3, square) << std::endl;
    return 0;
}
