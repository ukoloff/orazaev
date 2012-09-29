/*
 *      C++ homework. Problem 3.
 *      
 *      Due date: 04.09.12
 *
 *      Author: Aman Orazaev
 */
#ifndef AROAZAEV_DZ1_Z3_H
#define AORAZAEV_DZ1_Z3_H

#include<algorithm>



template <typename T>
void shift(T begin, T end) {
    if (end - begin <= 1)
        return;

    std::swap(*begin++, *--end);

    for (T it = (end); it != begin; --it) {
        std::swap(*it, *(it - 1));
    }
}

template <typename T>
void rotate(T begin, T end, int k) {
    if (begin == end)
        return;
    
    while (k < 0)
        k += end - begin;

    while(k-- > 0)
        shift(begin, end);
}



#endif /* AROAZAEV_DZ1_Z3_H */
