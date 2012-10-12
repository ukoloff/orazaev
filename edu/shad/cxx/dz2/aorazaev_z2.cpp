#include<iostream>
#include"aorazaev_z2.h"

int main() {
    {
    int a[5] = {0, 1, 2, 3, 4};
    Polynomial<int> p(1);
    Polynomial<int> q(a, a + 5);
    Polynomial<double> z(a, a + 5);
    Polynomial<int> t(q);

    std::cout << "P(x)<int> = " << p << std::endl;
    std::cout << "Q(x)<int> = " << q << std::endl;

    if (q == t) {
        std::cout << "Q<int> == T<int>" << std::endl;
    }

    if (q != p) {
        std::cout << "Q<int> != P<int>" << std::endl;
    }

    std::cout << "Q(x) += P(x)" << std::endl;

    q += p;
    std::cout << "Q(x) = " << q << std::endl;
    std::cout << "P(x) += Q(x)" << std::endl;
    p += q;
    std::cout << "P(x) = " << p << std::endl;
    std::cout << "P -= Q" << std::endl;
    p -= q;

    std::cout << "P(x) = " << p << std::endl;

    std::cout << p << " + " << (Polynomial<int>) 1<< std::endl;
    p += (Polynomial<int>) 1;
    std::cout << p << std::endl;
    std::cout << (Polynomial<int>) 1 << std::endl;
    }

    int a[2] = {0, 1};
    Polynomial<int> p(a, a + 2);
    int b[4] = {1, 2, 3 ,4};
    Polynomial<int> q(b, b + 4);

    std::cout << " ===> MULTIPLICATION:" << std::endl;
    std::cout << p << std::endl;
    std::cout << q << std::endl;
    p *= q;
    std::cout << p << std::endl;
    return 0;
}
