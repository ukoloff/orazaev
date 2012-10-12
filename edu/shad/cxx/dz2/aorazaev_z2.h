#ifndef AORAZAEV_CXX_DZ2_Z2
#define AORAZAEV_CXX_DZ2_Z2

#include<vector>
#include<cmath>
#include<ostream>

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
        while(begin != end) {
            coefficients.push_back(*begin++);
        }
        normalize();
    }

    T& operator[] (std::size_t n);
    T operator[] (std::size_t n) const;

    Polynomial<T> & operator += (const Polynomial<T> &);
    Polynomial<T> & operator -= (const Polynomial<T> &);
    Polynomial<T> & operator *= (const Polynomial<T> &);

    template <typename A>
    friend bool operator == (Polynomial<A>, Polynomial<A>);

    template <typename A>
    friend bool operator != (Polynomial<A>, Polynomial<A>);

    template <typename A>
    friend std::ostream & operator << (std::ostream &, const Polynomial<A> &);


private:
    std::vector<T> coefficients;

    void normalize() {
        for(size_t x = coefficients.size(); x > 1; --x) {
            if (std::abs(coefficients[x - 1]) <= EPSILON) {
                coefficients.pop_back();
            } else {
                break;
            }
        }
    }

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
    for(; i < coefficients.size(); ++i) {
        if (i == q.coefficients.size()) {
            return *this;
        }
        coefficients[i] += q[i];
    }
    for(; i < q.coefficients.size(); ++i) {
        coefficients.push_back(q[i]);
    }

    normalize();
    return *this;
}

template <typename T>
Polynomial<T> & Polynomial<T>::operator -= (const Polynomial<T> & q) {
    std::size_t i = 0;
    for(; i < coefficients.size(); ++i) {
        if (i == q.coefficients.size()) {
            return *this;
        }
        coefficients[i] -= q[i];
    }
    for(; i < q.coefficients.size(); ++i) {
        coefficients.push_back(q[i]);
    }

    normalize();
    return *this;
}

template <typename T>
Polynomial<T> & Polynomial<T>::operator *= (const Polynomial<T> & q) {
    size_t oldSize = coefficients.size();
    coefficients.resize(coefficients.size() + q.coefficients.size() - 1, 0);
    for(int i = coefficients.size(); i >= 0; ++i) {
        T res = 0;
        for(int j = i; j >= 0; --j) {
            if (i - j == q.coefficients.size()) {
                break;
            }
            res += coefficients[i] * q[i - j];
        }
        coefficients[i] = res;
    }

    normalize();
    return *this;
}

template <typename A>
bool operator == (Polynomial<A> p, Polynomial<A> q) {
    p.normalize(); q.normalize();
    return p.coefficients == q.coefficients;
}

template <typename A>
bool operator != (Polynomial<A> p, Polynomial<A> q) {
    p.normalize(); q.normalize();
    return p.coefficients != q.coefficients;
}


// If DEBUG defined, print python-like list.
template <typename A>
std::ostream& operator << (
    std::ostream & out,
    const Polynomial<A> & p)
{
#   ifdef DEBUG
    out << "[";
#   endif

    for(size_t t = 0; t < p.coefficients.size(); ++t) {
        out << p[t];
#       ifndef DEBUG
        out << " * x^" << t;
#       endif

        if (t + 1 != p.coefficients.size()) {
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

#endif /* AORAZAEV_CXX_DZ2_Z2 */
