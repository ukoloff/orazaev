#ifndef TPTR_H
#define TPTR_H

template <class T>
class TPtr {
    T*              pointed_data;
    unsigned int*   refcount;
public:
    TPtr()
        : pointed_data(0)
        , refcount(0) {
    }

    TPtr(T* t)
        : pointed_data(t)
        , refcount(new unsigned(1)) {
    }

    TPtr(const TPtr<T>& p) {
        refcount = p.refcount;
        if (refcount)
            *refcount += 1;
        pointed_data = p.pointed_data;
    }

    TPtr<T>& operator=(const TPtr<T>& p) throw() {
        if (&p != this) {
            if (refcount) {
                *refcount -= 1;
                if (*refcount == 0) {
                    delete pointed_data;
                    delete refcount;
                }
            }
    
            refcount = p.refcount;
            if (refcount)
                *refcount += 1;
            pointed_data = p.pointed_data;
        }
    }

    T* operator->() throw() {
        return pointed_data;
    }
    
    const T* operator->() const throw() {
        return pointed_data;
    }
    
    T& operator*() const throw() {
        return *pointed_data;
    }

    T* get() throw() {
        return pointed_data;
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
                delete pointed_data;
                delete refcount;
            }
        }
    }
};

#endif /* TPTR_H */
