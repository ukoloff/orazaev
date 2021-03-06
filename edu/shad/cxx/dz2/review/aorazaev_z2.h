/*
 *      C++ homework. Problem 2.
 *
 *      Due date: 25.09.12
 *
 *      Author: Aman Orazaev
 */
#ifndef EDU_SHAD_CXX_DZ2_AORAZAEV_Z2_H_
#define EDU_SHAD_CXX_DZ2_AORAZAEV_Z2_H_

#include <vector>
#include <cmath>
#include <ostream>

// need for demonstration
#include <iostream>

const double EPSILON = 0.000000001;

template <typename T>
class Polynomial {
    public:
    Polynomial()
        : coefficients(1, 0)
    { }

    Polynomial(T freeMember)
        : coefficients(1, freeMember)
    { }

    template <typename TIterator>
    Polynomial(TIterator begin, TIterator end) {
        while (begin != end) {
            coefficients.push_back(*begin++);
        }
        normalize();
    }

    T& operator[] (std::size_t n);
    T operator[] (std::size_t n) const;

    Polynomial<T> & operator += (const Polynomial<T> &);
    Polynomial<T> & operator -= (const Polynomial<T> &);
    Polynomial<T> & operator *= (const Polynomial<T> &);
    Polynomial<T> operator + (const Polynomial<T> &) const;
    Polynomial<T> operator - (const Polynomial<T> &) const;
    Polynomial<T> operator * (const Polynomial<T> &) const;

    Polynomial<T> & operator /= (const Polynomial<T> & q);
    Polynomial<T> & operator %= (const Polynomial<T> &);
    Polynomial<T> operator / (const Polynomial<T> &) const;
    Polynomial<T> operator % (const Polynomial<T> &) const;

    Polynomial<T> operator , (const Polynomial<T> &) const;

    T operator() (const T &) const;

    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::iterator iterator;

    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

    template <typename A>
    friend bool operator == (Polynomial<A>, Polynomial<A>);

    template <typename A>
    friend bool operator != (Polynomial<A>, Polynomial<A>);

    template <typename A>
    friend std::ostream & operator << (std::ostream &, const Polynomial<A> &);




    private:
    std::vector<T> coefficients;

    void normalize();
    void normalizeElements();
    void division(Polynomial<T> * const p,
        const Polynomial<T> & q,
        Polynomial<T> * const mod);

    size_t size() const;

    Polynomial<T> * gcd(Polynomial<T> *, Polynomial<T> *) const;
    T gcd(T, T) const;
};






template <typename T>
T& Polynomial<T>::operator[] (std::size_t n) {
    if (n >= coefficients.size()) {
        coefficients.resize(n + 1, 0);
    }
    return coefficients[n];
}



template <typename T>
T Polynomial<T>::operator[] (std::size_t n) const {
    if (n >= coefficients.size()) {
        return 0;
    }
    return coefficients[n];
}



template <typename T>
Polynomial<T> & Polynomial<T>::operator += (const Polynomial<T> & q) {
    std::size_t i = 0;
    for (; i < coefficients.size(); ++i) {
        if (i == q.coefficients.size()) {
            return *this;
        }
        coefficients[i] += q[i];
    }
    for (; i < q.coefficients.size(); ++i) {
        coefficients.push_back(q[i]);
    }

    normalize();
    return *this;
}



template <typename T>
Polynomial<T> & Polynomial<T>::operator -= (const Polynomial<T> & q) {
    std::size_t i = 0;
    for (; i < coefficients.size(); ++i) {
        if (i == q.coefficients.size()) {
            return *this;
        }
        coefficients[i] -= q[i];
    }
    for (; i < q.coefficients.size(); ++i) {
        coefficients.push_back(q[i]);
    }

    normalize();
    return *this;
}



template <typename T>
Polynomial<T> & Polynomial<T>::operator *= (const Polynomial<T> & q) {
    size_t oldSize = coefficients.size();
    coefficients.resize(coefficients.size() + q.coefficients.size() - 1, 0);

    for (int i = coefficients.size(); i > 0; --i) {
        T res = 0;
        for (int j = i; j > 0; --j) {
            if (i - j == q.coefficients.size()) {
                break;
            }
            res += coefficients[j - 1] * q[i - j];
        }
        coefficients[i - 1] = res;
    }

    normalize();
    return *this;
}



template <typename T>
Polynomial<T> Polynomial<T>::operator + (const Polynomial<T> & p) const {
    Polynomial<T> ans(*this);
    return ans += p;
}



template <typename T>
Polynomial<T> Polynomial<T>::operator - (const Polynomial<T> & p) const {
    Polynomial<T> ans(*this);
    return ans -= p;
}



template <typename T>
Polynomial<T> Polynomial<T>::operator * (const Polynomial<T> & p) const {
    Polynomial<T> ans(*this);
    return ans *= p;
}



template <typename T>
Polynomial<T> & Polynomial<T>::operator /= (const Polynomial<T> & q) {
    Polynomial<T> mod(*this);

    division(this, q, &mod);
    normalize();

    return *this;
}


template <typename T>
Polynomial<T> Polynomial<T>::operator / (const Polynomial<T> & q) const {
    Polynomial<T> res(*this);

    return res /= q;
}



template <typename T>
Polynomial<T> & Polynomial<T>::operator %= (const Polynomial<T> & q) {
    Polynomial<T> num(*this);

    division(&num, q, this);
    normalize();

    return *this;
}



template <typename T>
Polynomial<T> Polynomial<T>::operator % (const Polynomial<T> & q) const {
    Polynomial<T> res(*this);

    return res %= q;
}



template <typename T>
Polynomial<T> Polynomial<T>::operator , (const Polynomial<T> & q) const {
    Polynomial<T> p(*this);
    Polynomial<T> t(q);

    gcd(&p, &t);
    return p == Polynomial<T>(0) ? t : p;
}



template <typename T>
T Polynomial<T>::operator() (const T & x) const {
    T res = coefficients[0];
    T pow = x;

    for (const_iterator it = begin() + 1;
        it != end(); ++it) {
        res += *it * pow;
        pow *= x;
    }

    return res;
}



template <typename T>
typename Polynomial<T>::const_iterator Polynomial<T>::begin() const {
    return coefficients.begin();
}



template <typename T>
typename Polynomial<T>::const_iterator Polynomial<T>::end() const {
    return coefficients.end();
}



template <typename T>
typename Polynomial<T>::iterator Polynomial<T>::begin() {
    return coefficients.begin();
}



template <typename T>
typename Polynomial<T>::iterator Polynomial<T>::end() {
    return coefficients.end();
}



template <typename A>
bool operator == (Polynomial<A> p, Polynomial<A> q) {
    p.normalize();
    q.normalize();
    return p.coefficients == q.coefficients;
}



template <typename A>
bool operator != (Polynomial<A> p, Polynomial<A> q) {
    p.normalize();
    q.normalize();
    return p.coefficients != q.coefficients;
}



// If DEBUG defined, print python-like list.
template <typename A>
std::ostream& operator << (
    std::ostream & out,
    const Polynomial<A> & p) {
#   ifdef DEBUG
    out << "[";
#   endif

    for (typename Polynomial<A>::const_iterator it = p.begin();
        it != p.end(); ++it) {
        out << *it;
#       ifndef DEBUG
        out << " * x^" << it - p.begin();
#       endif

        if (it + 1 != p.end()) {
#       ifndef DEBUG
            out << " + ";
#       else
            out << ", ";
#       endif
        }
    }

#   ifdef DEBUG
    out << "]";
#   endif
    return out;
}



template <typename T>
void Polynomial<T>::normalize() {
    for (size_t x = coefficients.size(); x > 1; --x) {
        if (std::abs(coefficients[x - 1]) <= EPSILON) {
            coefficients.pop_back();
        } else {
            break;
        }
    }
}



template <typename T>
void Polynomial<T>::normalizeElements() {
    T cd = 0;
    for (size_t i = 0; i < size(); ++i) {
        cd = gcd(coefficients[i], cd);
    }

    if (cd != 1) {
        for (size_t i = 0; i < size(); ++i) {
            coefficients[i] /= cd;
        }
    }
}



template <typename T>
void Polynomial<T>::division(Polynomial<T> * const p
    , const Polynomial<T> & q
    , Polynomial<T> * const mod) {
    if (q.size() > p->size()) {
        *mod = *p;
        p->coefficients = std::vector<T>(1, 0);
        return;
    }

    p->coefficients.resize(p->size() - q.size() + 1);
    p->coefficients.assign(p->size(), 0);

    while (mod->size() > q.size() ||
           ((*mod)[mod->size() - 1] / q[q.size() - 1] != 0 &&
           mod->size() == q.size()))
    {
        size_t cur = mod->size() - q.size();

        (*p)[cur] =
            (*mod)[mod->size() - 1] / q[q.size() - 1];


        *mod -= q * Polynomial<T>(p->begin(), p->begin() + cur + 1);

        if ((*p)[cur] == 0) {
            break;
        }
    }
}



template <typename T>
size_t Polynomial<T>::size() const {
    return coefficients.size();
}



template <typename T>
Polynomial<T> * Polynomial<T>::gcd(Polynomial<T> * p, Polynomial<T> * q) const {
    if (*q == Polynomial<T>(0)) {
        return p;
    }

    // Fix infinity recursion for T == int
    // Example: gcd([0, 0, 2], [0, 0, 0, 1])
    if (*p / *q == Polynomial<T>(0) && *p != Polynomial<T>(0)) {
        if (*q / *p == Polynomial<T>(0)) {
            p->normalizeElements();
            q->normalizeElements();
        }
    }

    *p %= *q;

    return gcd(q, p);
}



template <typename T>
T Polynomial<T>::gcd(T x, T y) const {
    if (y == static_cast<T> (0)) {
        return x;
    }

    return gcd(y, x % y);
}




template <typename T>
struct PolynomialConstructHelper {
    T coefficient;

    PolynomialConstructHelper()
            : coefficient(1) {
    }



    PolynomialConstructHelper(const T & c)
            : coefficient(c) {
    }



    Polynomial<T> operator ^ (size_t degree) const {
        std::vector<T> temp(degree + 1, 0);
        temp.back() = coefficient;

        return Polynomial<T>(temp.begin(), temp.end());
    }
};



// Help for: (1 * y^3) + (2 * y^19) + ...
template <typename T>
PolynomialConstructHelper<T> operator * (const T & coeff
        , const PolynomialConstructHelper<T> & x) {
    return PolynomialConstructHelper<T>(coeff);
}


// Help for: 1 * (y^3) + 2 * (y^19) + ...
template <typename T>
Polynomial<T> operator * (const T & lhs, const Polynomial<T> & rhs) {
    return rhs * lhs;
}



void polynomialDemo() {
    int a[5] = {
        0, 1, 2, 3, 4
    };

    Polynomial<int> p(a, a + 5); // [0, 1, 2, 3, 4]

    Polynomial<int> b(1); // [1]

    std::cout << p + b << std::endl;
    // 1 * x^0 + 1 * x^1 + 2 * x^2 + 3 * x^3 + 4 * x^4

    Polynomial<int> t(p / b);

    if (t == p) {
        std::cout << p / t << std::endl;
        // 1 * x^0
    }

    Polynomial<int> c(2); // [2]

    std::cout << p * c << std::endl;
    // 0 * x^0 + 2 * x^1 + 4 * x^2 + 6 * x^3 + 8 * x^4

    Polynomial<int> x(a, a + 2); // [0, 1]

    std::cout << (p, x) << std::endl;
    // 0 * x^0 + 1 * x^1


    PolynomialConstructHelper<int> y;

    Polynomial<int> constructDemonstration =
            1 * (y^0) +  4 * (y^5) + 10 * (y^3);

    std::cout << constructDemonstration << std::endl;

    Polynomial<int> constructDemonstration2 = 199 * y^19;

    std::cout << constructDemonstration2 << std::endl;
}

#endif  // EDU_SHAD_CXX_DZ2_AORAZAEV_Z2_H_
