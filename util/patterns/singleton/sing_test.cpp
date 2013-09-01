#include <iostream>
#include <iostream>
#include <string>

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

void string_print(std::string s) {
    std::cout << s << std::endl;
}

int main() {
    TSingleton<int> si;
    std::cout << si << std::endl;

    TSingleton<int> si2(new int(10));
    
    std::cout << si << std::endl;
    

    TSingleton<std::string> ss(new std::string("stroka odna"));
        
    std::cout << ss.inst() << std::endl;

    string_print(ss);
    //std::cout.operator<<(ss); std::cout << std::endl;
    //ss = std::string("Amazing!");
    std::cout << TSingleton<std::string>();
}
