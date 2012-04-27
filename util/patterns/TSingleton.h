#ifndef TSINGLETON_H
#define TSINGLETON_H

#include <iostream>

template <class T>
class TSingleton {
    static T* instance;
    static int count;

    TSingleton(const TSingleton & ts) {};
    TSingleton operator=(const TSingleton& s) {};
public:
    TSingleton();
    TSingleton(T*);
    virtual ~TSingleton();

    T* operator->() { return instance; }
    operator T() { return *instance; }

    TSingleton& operator=(const T&);

    static T* inst() { return instance; }
    static int getRefCount() { return count; }

    inline T operator++() { return ++(*instance); }
    inline T operator++(int) { return (*instance)++; }
    inline T operator--() { return --(*instance); }
    inline T operator--(int) { return (*instance)--; }
    inline T operator-=(const T& t) { return *instance = (*instance) - t; }
    inline T operator+=(const T& t) { return *instance = (*instance) + t; }
    inline T operator*=(const T& t) { return *instance = (*instance) * t; }
    inline T operator/=(const T& t) { return *instance = (*instance) / t; }
    inline T operator%=(const T& t) { return *instance = (*instance) % t; }
    inline T operator|=(const T& t) { return *instance = (*instance) | t; }
    inline T operator&=(const T& t) { return *instance = (*instance) & t; }
    //inline bool operator!() { return !(*instance); }
};


// TSingleton realization
template <class T>
T* TSingleton<T>::instance = 0;

template <class T>
int TSingleton<T>::count = 0;

template <class T>
TSingleton<T>::TSingleton() {
    if (!count)
        instance = new T();
    count++;
}

template <class T>
TSingleton<T>::TSingleton(T* t) {
    if (!count)
        instance = t;
    else if (instance != t) {
        delete instance;
        instance = t;
    }
    count++;
}

template <class T>
TSingleton<T>::~TSingleton() {
    count--;
    if (count == 0) {
        delete instance;
        instance = 0;
    }
}

template <class T>
TSingleton<T>& TSingleton<T>::operator=(const T& t) {
    if (&t != instance) {
        delete instance;
        instance = new T(t);
    }
    return *this;
}



// func tools
template <class T>
inline std::ostream& operator<<(std::ostream& os, const TSingleton<T>& s) {
    return os << *(s.inst());
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, TSingleton<T>& s) {
    return os << *(s.inst());
}

#endif
