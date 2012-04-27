#ifndef TSINGLETON_H
#define TSINGLETON_H

#include <iostream>

template <class T> class TSingleton;

template <class T>
inline std::ostream& operator<<(std::ostream& os, const TSingleton<T>& s) {
    T x = s.inst();
    return os << x;
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, TSingleton<T>& s) {
    T x = s.inst();
    return os << x;
}

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

    TSingleton operator=(T);

    static T inst() { return *instance; }
    static int getRefCount() { return count; }
};

template <class T>
T* TSingleton<T>::instance = 0;

template <class T>
int TSingleton<T>::count = 0;

template <class T>
TSingleton<T>::TSingleton() {
    if (!instance)
        instance = new T();
    count++;
}

template <class T>
TSingleton<T>::TSingleton(T* t) {
    if (!instance)
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
    if (count == 0) delete instance;
}

template <class T>
TSingleton<T> TSingleton<T>::operator=(T t) {
    if (&t != instance) {
        delete instance;
        instance = new T(t);
    }
    return this;
}

#endif
