#ifndef TPTR_H
#define TPTR_H
#include <iostream>

template <class T>
class TPtr {
    T*              data;
    unsigned int*   refcount;
public:
    TPtr()
        : data(0)
        , refcount(0) {
    }

    TPtr(T* t)
        : data(t)
        , refcount(new unsigned(1)) {
    }

    TPtr(const TPtr<T>& p) {
        refcount = p.refcount;
        if (refcount)
            *refcount += 1;
        data = p.data;
    }

    TPtr<T>& operator=(const TPtr<T>& p) throw() {
        if (&p != this) {
            if (refcount) {
                *refcount -= 1;
                if (*refcount == 0) {
                    delete data;
                    delete refcount;
                }
            }
    
            refcount = p.refcount;
            if (refcount)
                *refcount += 1;
            data = p.data;
        }
    }

    T* operator->() const throw() {
        return &data;
    }
    
    T& operator*() const throw() {
        return *data;
    }

    T* get() throw() {
        return data;
    }

    int getRefCount() const throw() {
        if (!refcount)
            return 0;
        return *refcount;
    }

    virtual ~TPtr() throw() {
        if (refcount != 0) {
            *refcount -= 1;
            if (*refcount == 0) {
                delete data;
                delete refcount;
            }
        }
    }
};

#endif /* TPTR_H */
