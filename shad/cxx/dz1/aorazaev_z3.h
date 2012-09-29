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

// Need for demo().
#include<vector>
#include<iostream>



template <typename T>
void shift(T begin, T end) {
    if (end - begin <= 1)
        return;

    std::swap(*begin++, *--end);

    for (T it = end; it != begin; --it) {
        std::swap(*it, *(it - 1));
    }
}

template <typename T>
void rotate(T begin, T end, int k) {
    while (k < 0)
        k += end - begin;

    while(k-- > 0)
        shift(begin, end);
}


// Rotate demonstration using
// std::vector and array
void demo() {
    int a[5] = {0, 1, 2, 3, 4};
    std::vector<int> v(a, a + 5);
    
    rotate(a, a + 5, 3);
    rotate(v.begin(), v.end(), -3);

    std::cout << "rotate(vector(0..4), -3): ";
    for(std::vector<int>::const_iterator it = v.begin();
        it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;

    std::cout << "rotate(array(0..4), 3): ";
    for(int i = 0; i < 5; ++i)
        std::cout << a[i] << " ";
    std::cout << std::endl;
}


#endif /* AROAZAEV_DZ1_Z3_H */
