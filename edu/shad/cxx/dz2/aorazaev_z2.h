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
    Polynomial<T> operator + (const Polynomial<T> &) const;
    Polynomial<T> operator - (const Polynomial<T> &) const;
    Polynomial<T> operator * (const Polynomial<T> &) const;

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

    for(int i = coefficients.size(); i > 0; --i) {
        T res = 0;
        for(int j = i; j > 0; --j) {
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
T Polynomial<T>::operator() (const T & x) const {
    T res = coefficients[0];
    T pow = x;

    for(const_iterator it = begin() + 1;
        it != end(); ++it)
    {
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

    for(typename Polynomial<A>::const_iterator it = p.begin();
        it != p.end(); ++it)
    {
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
    for(size_t x = coefficients.size(); x > 1; --x) {
        if (std::abs(coefficients[x - 1]) <= EPSILON) {
            coefficients.pop_back();
        } else {
            break;
        }
    }
}


#endif /* AORAZAEV_CXX_DZ2_Z2 */
