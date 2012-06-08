#include "../tptr.h"
#include "../tptr.h"
#include <string>
#include <iostream>
#include <vector>

void func(TPtr<std::string> pts) {
    *pts = "Func changes.";
}

std::string addSpaces(std::string s, int x = 50) {
    if (s.size() < x )
        s.resize(x, '.');
    return s;
}


int main() {
    std::cout << addSpaces("CONSTRUCTION WITHOUT ARGS");
    TPtr<std::string> a;
    TPtr<std::string> b;
    TPtr<std::string> c;
    std::cout << " [ PASS ]" << std::endl;



    std::cout << addSpaces("CONSTRUCTION WITH ARGS");
    TPtr<std::string> d(new std::string("Hello, memory!"));
    TPtr<int> e(new int(777));
    std::cout << " [ PASS ]" << std::endl;



    std::cout << addSpaces("REFCOUNT TEST");
    c = d;
    if (a.getRefCount() != 0 || b.getRefCount() != 0 || c.getRefCount() != 2 || d.getRefCount() != 2 || e.getRefCount() != 1)
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;
    


    std::cout << addSpaces("DESTRUCTION AND COPY CONSTRUCTION");
    c = TPtr<std::string>();
    {
        TPtr<std::string> tempd = d;
    }
    if (c.getRefCount() != 0 || d.getRefCount() != 1)
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;



    std::cout << addSpaces("OPERATIONS");
    *e -= 444;
    *d = std::string("Hello, memory! I'm here!");
    a = b;
    c = b;
    a = c;
    b = TPtr<std::string>(new std::string("Dolly"));
    if (*e != 333 || *d != "Hello, memory! I'm here!" || a.getRefCount() != 0 || b.getRefCount() != 1 || c.getRefCount() != 0 || d.getRefCount() != 1 || *(d.get()) != "Hello, memory! I'm here!")
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;



    std::cout << addSpaces("AS FUNCTION ARG");
    std::vector<TPtr<int> > pvector;
    pvector.push_back(e);
    pvector.push_back(e);
    pvector.erase(pvector.begin() + 1);
    func(d);
    if (e.getRefCount() != 2 || *pvector[0] != 333 || pvector.size() != 1 || d.getRefCount() != 1 || *d != "Func changes.")
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;
    
    
    return 0;
}
