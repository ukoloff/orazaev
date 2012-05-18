#ifndef TESTER_H
#define TESTER_H

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

//Pair class
template< class A, class B >
class TPair {
    A first;
    B second;
public:
    TPair()
        : first()
        , second() {
    }
    
    TPair(A cfirst, B csecond)
        : first(cfirst)
        , second(csecond) {
    }

    inline A& First() { return first; }
    inline B& Second() { return second; }
    const B& Second() const { return first; }
    const A& First() const { return second; }
};


//TPair func tools
template< class A, class B >
inline std::ostream& operator<<(std::ostream& os, const TPair<A, B>& pair) {
    os << "(" << (A) pair.First() << ", " << (B) pair.Second() << ")";
    return os;
}

template< class A, class B >
inline std::ostream& operator<<(std::ostream& os, TPair<A, B>& pair) {
    os << "(" << (A) pair.First() << ", " << (B) pair.Second() << ")";
    return os;
}


/*
//Catalog class
//is vector wrapper
template< class T >
class TCatalog {
    std::vector<T> data;
    public:
    TCatalog() : data() {};
    TCatalog() : data() 
};
*/

namespace Tester {

    template< class T >
    void ptest(const T& x, const T& y) {
        bool res = (x == y);
        std::string ok;

        std::ostringstream out(std::ostringstream::out);

        out << x << " == " << y;
        ok = out.str();
        ok.resize(70, ' ');
        
        ok += res ? "[TRUE]" : "[FALSE]";

        std::cout << ok << std::endl;
    }


    template< class T >
    inline bool test(T x, T y) {
        return (x == y);
    }

    template< class T >
    inline void ptest(const TPair<T, T>& p) {
        ptest(p.First(), p.Second());
    }

    template< class T >
    inline bool test(const TPair<T, T>& p) {
        return test(p.First(), p.Second());
    }

    template< class T >
    void pntest(int n, TPair<T, T>* pairmas) {
        for(int x = 0; x < n; x++) {
            ptest(pairmas[x]);
        }        
    }

    template< class T >
    bool ntest(int n, TPair<T, T>* pairmas) {
        for(int x = 0; x < n; x++)
            if (!test(pairmas[x]))
                return false;

        return true;
    }

} /* namespace Tester */

#endif /* TESTER_H */
