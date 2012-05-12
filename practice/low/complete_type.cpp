#include "complete_type.h"

template <class T>
inline void checkDelete(T* t) {
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
    delete t;
}

//class S; 

int main() {
    S* s = new S();
    checkDelete(s);
}
