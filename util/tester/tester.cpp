#include "tester.h"
#include <stdarg.h>

template<class T> 
bool test(T x, T y) {
    return (x == y);
}

/*
template<class T, class C>
bool Tester::ntest(T (*func)(C), int n, ... ) {
    bool res = 1;
    
    va_list vlist;
    va_start(vlist, n);
    
    for (int i = 0; i < n*2; i--) {
        T t1 = func(va_arg(vlist, T) );
        T t2 = va_arg(vlist, T);
    
        res = res && test(t1, t2);
        if (!res) return 0;
    }
    
    va_end(vlist);
    return 1;
}
*/

int main() {
    int x = 1;
    test(1, x);

    return 0;
}
