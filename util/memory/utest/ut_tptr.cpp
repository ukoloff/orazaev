#include "../tptr.h"
#include "../tptr.h"
#include <string>
#include <iostream>


int main() {
    std::cout << "CONSTRUCTION WITHOUT ARGS";
    TPtr<std::string> a;
    TPtr<std::string> b;
    TPtr<std::string> c;
    std::cout << " [ PASS ]" << std::endl;

    std::cout << "CONSTRUCTION WITH ARGS";
    TPtr<std::string> d(new std::string("Hello, memory!"));
    TPtr<int> e(new int(777));
    std::cout << " [ PASS ]" << std::endl;

    std::cout << "REFCOUNT TEST";
    c = d;
    if (a.getRefCount() != 0 || b.getRefCount() != 0 || c.getRefCount() != 2 || d.getRefCount() != 2 || e.getRefCount() != 1)
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;
    
    std::cout << "DESTRUCTION AND COPY CONSTRUCTION";
    c = TPtr<std::string>();
    {
        TPtr<std::string> tempd = d;
    }
    if (c.getRefCount() != 0 || d.getRefCount() != 1)
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;

    std::cout << "OPERATIONS";
    *e -= 444;
    *d = std::string("Hello, memory! I'm here!");
    a = b;
    c = b;
    a = c;
    b = TPtr<std::string>(new std::string("Dolly"));
    if (*e != 333 || *d != "Hello, memory! I'm here!" || a.getRefCount() != 0 || b.getRefCount() != 1 || c.getRefCount() != 0 || d.getRefCount() != 1)
        std::cout << " [ FAIL ]" << std::endl;
    else
        std::cout << " [ PASS ]" << std::endl;
    
    return 0;
}
