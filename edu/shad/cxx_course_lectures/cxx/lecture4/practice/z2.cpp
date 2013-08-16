#include<iostream>
#include<map>

double minus(double x, double y) {
    return x - y;
}

double sum(double x, double y) {
    return x + y;
}

double multiplication(double x, double y) {
    return x * y;
}

double division(double x, double y) {
    return x / y;
}

int main() {
    double (*calc[4]) (double, double);
    calc[0] = sum;
    calc[1] = minus;
    calc[2] = multiplication;
    calc[3] = division;
    while (std::cin.good()) {
        double x, y;
        size_t operation;
        std::cin >> x >> y;
        std::cin >> operation;
        std::cout << "result: " << (calc[operation])(x, y) << std::endl;
    }

    return 0;
}
