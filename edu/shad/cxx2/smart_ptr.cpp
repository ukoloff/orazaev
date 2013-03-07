/**
 *  C++ course. Homework 1. Problem 2.
 *
 *  @brief TSmartPointer<T> class realization.
 *
 *  @author Aman Orazaev
 */
#include <iostream>



template <typename T>
class TDefaultStoragePolicy;

template <typename T>
class TNonCopyableOwnershipPolicy;



template <
    typename T,
    template <typename> class TStoragePolicy = TDefaultStoragePolicy,
    template <typename> class TOwnershipPolicy = TNonCopyableOwnershipPolicy
>
class TSmartPointer : public TStoragePolicy<T> {
public:
    TSmartPointer()
        : TStoragePolicy<T>()
        , ownershipPolicyImpl() { }

    TSmartPointer(const typename TStoragePolicy<T>::TStoredType& pointer)
        : TStoragePolicy<T>(pointer)
        , ownershipPolicyImpl() { }

    TSmartPointer(const TSmartPointer& other) {
        ownershipPolicyImpl.Clone(other);
    }

    TSmartPointer& operator=(const TSmartPointer& rhs) {
        if (*this != rhs) {
            ownershipPolicyImpl.Release(*this);
            ownershipPolicyImpl.Clone(rhs);
        }
        return *this;
    }

    ~TSmartPointer() { TStoragePolicy<T>::Destroy(); }
private:
    TOwnershipPolicy<
        TSmartPointer<T, TStoragePolicy, TOwnershipPolicy>
    > ownershipPolicyImpl;
};



template <typename T>
class TDefaultStoragePolicy {
protected:
    typedef T* TStoredType;
    typedef T* TPointerType;

    typedef T& TReferenceType;

public:
    TDefaultStoragePolicy()
        : _pointer(Default()) { }

    TDefaultStoragePolicy(const TStoredType& pointer)
        : _pointer(pointer) { }

    TPointerType operator->() const { return _pointer; }
    TReferenceType operator*() const { return *_pointer; }

    friend inline TPointerType GetImpl(
            const TDefaultStoragePolicy& sp) {
        return sp._pointer;
    }

    friend inline const TStoredType& GetImplRef(
            const TDefaultStoragePolicy& sp) {
        return sp._pointer;
    }

    friend inline TStoredType& GetImplRef(
            TDefaultStoragePolicy& sp) {
        return sp._pointer;
    }

protected:
    void Destroy() { delete _pointer; } 
    static TStoredType Default() { return 0; }

    ~TDefaultStoragePolicy() { }

protected:
    TStoredType _pointer;
};



template <typename T>
class TArrayStoragePolicy : public TDefaultStoragePolicy<T> {
public:
    TArrayStoragePolicy() : TDefaultStoragePolicy<T>() { }
    TArrayStoragePolicy(
            const typename TDefaultStoragePolicy<T>::TStoredType& pointer)
        : TDefaultStoragePolicy<T>(pointer) { }

protected:
    void Destroy() { delete[] TDefaultStoragePolicy<T>::_pointer; }
};


template <typename T>
class TNonCopyableOwnershipPolicy {
private:
    T Clone(const T& val);
    bool Release(const T& val);
};



class Foo {
public:
    int x;
    char y;

public:
    Foo(int x, char y)
        : x(x), y(y) { }
};



int main() {
    TSmartPointer<int> sp(new int(20));

    std::cout << *sp << std::endl;

    TSmartPointer<Foo> foo(new Foo(*sp, 78));

    std::cout << "foo = {" << foo->x << ", '"
              << foo->y << "'}\n";

    return 0;
}
