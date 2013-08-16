#include<iostream>

int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }

   return gcd(b % a, a);
}

class RNumber {
    int num;
    int denum;

    void normalize() {
        int cd = gcd(num, denum);
        num /= cd;
        denum /= cd;
    }

public:
    RNumber(int n, int d)
        : num(n), denum(d)
    { 
        normalize();
    }

    int numerator() const {
        return num;
    }
    
    int denominator() const {
        return denum;
    }

    RNumber & operator += (const RNumber & x) {
        num = num * x.denominator() + x.numerator() * denum;
        denum = denum * x.denominator();
        normalize();
        return *this;
    }

    RNumber & operator -= (const RNumber & x) {
        num = num * x.denominator() - x.numerator() * denum;
        denum = denum * x.denominator();
        normalize();
        return *this;
    }

    RNumber & operator *= (const RNumber & x) {
        num *= x.numerator();
        denum *= x.denominator();
        normalize();
        return *this;
    }

    RNumber & operator /= (const RNumber & x) {
        num *= x.denominator();
        denum *= x.numerator();
        normalize();
        return *this;
    }
    
    RNumber operator + (const RNumber & x) const {
        RNumber ans(*this);
        return ans += x;
    }

    RNumber operator - (const RNumber & x) const {
        RNumber ans(*this);
        return ans -= x;
    }

    RNumber operator * (const RNumber & x) const {
        RNumber ans(*this);
        return ans *= x;
    }

    RNumber operator / (const RNumber & x) const {
        RNumber ans(*this);
        return ans /= x;
    }

    RNumber & operator ++() {
        num += denum;
        return *this;
    }

    RNumber & operator --() {
        num -= denum;
        return *this;
    }

    RNumber operator ++(int a) {
        RNumber old(*this);
        num += denum;
        return old;
    }

    RNumber operator --(int a) {
        RNumber old(*this);
        num -= denum;
        return old;
    }
};

bool operator < (RNumber x, RNumber y) {
    return x.numerator() * y.denominator() < y.numerator() * x.denominator();
}

bool operator == (RNumber x, RNumber y) {
    return x.numerator() == y.numerator() && x.denominator() == y.denominator();
}

bool operator != (RNumber x, RNumber y) {
    return !(x == y);
}

bool operator <= (RNumber x, RNumber y) {
    return x < y || x == y;
}

bool operator > (RNumber x, RNumber y) {
    return !(x <= y);
}

bool operator >= (RNumber x, RNumber y) {
    return x > y || x == y;
}

int main() {
    RNumber x(10, 50), y(4, 5);

    std::cout << x.numerator() << "/" << x.denominator() << std::endl;
    x += y;
    std::cout << x.numerator() << "/" << x.denominator() << std::endl;
    x /= y;
    std::cout << x.numerator() << "/" << x.denominator() << std::endl;
    x *= y;
    std::cout << x.numerator() << "/" << x.denominator() << std::endl;
}
