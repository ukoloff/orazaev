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

    TSmartPointer(const TSmartPointer& other)
        : TStoragePolicy<T>()
        , ownershipPolicyImpl(other.ownershipPolicyImpl) {
        GetImplRef(*this) = other.ownershipPolicyImpl.Clone(GetImplRef(other));
    }

    TSmartPointer& operator=(const TSmartPointer& rhs) {
        if (GetImplRef(*this) == GetImplRef(rhs)) {
            return *this;
        }
        TSmartPointer tmp(rhs);
        Swap(tmp);
        return *this;
    }

    ~TSmartPointer() {
        TryToDestory();
    }

private:
    void TryToDestory() {
        if (ownershipPolicyImpl.Release(GetImplRef(*this))) {
            TStoragePolicy<T>::Destroy();
        }
    }

    void Swap(TSmartPointer& rhs) throw() {
        typename TStoragePolicy<T>::TStoredType tmpPointer(GetImpl(*this));
        GetImplRef(*this) = GetImpl(rhs);
        GetImplRef(rhs) = tmpPointer;

        TOwnershipPolicy<typename TStoragePolicy<T>::TStoredType>
            tmpPolicy(ownershipPolicyImpl);
        ownershipPolicyImpl = rhs.ownershipPolicyImpl;
        rhs.ownershipPolicyImpl = tmpPolicy;
    }

private:
    TOwnershipPolicy<
        typename TStoragePolicy<T>::TStoredType
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
    T Clone(const T&) const;

public:
    bool Release(const T&) {
        return true;
    }
};



template <typename T>
class TRefCountOwnershipPolicy {
public:
    TRefCountOwnershipPolicy()
        : _pCount(new unsigned int(1)){ }

    T Clone(const T& val) const {
        ++*_pCount;
        return val;
    }

    bool Release(const T&) {
        if (!--*_pCount) {
            delete _pCount;
            return true;
        }
        return false;
    }

private:
    unsigned int* _pCount;
};



template <typename T>
class TDestructiveCopyOwnershipPolicy {
public:
    T Clone(const T& val) const {
        T tmp(val);
        const_cast<T&>(val) = 0;
        return tmp;
    }

    bool Release(const T&) { return true; }
};



template <typename T>
class TRefLinkedOwnershipPolicy {
public:
    TRefLinkedOwnershipPolicy()
        : _next(0)
        , _prev(0) { }

    TRefLinkedOwnershipPolicy(const TRefLinkedOwnershipPolicy& other)
        : _next(0)
        , _prev(0) {
        _next = &const_cast<TRefLinkedOwnershipPolicy&>(other);
        _prev = other._prev;
        if (_prev) {
            _prev->_next = this;
        }
        _next->_prev = this;
    }

    T Clone(const T& val) const {
        return val;
    }

    bool Release(const T&) {
        if (_next == _prev) {
            return true;
        }
        if (_prev)
            _prev->_next = _next;
        if (_next)
            _next->_prev = _prev;

        return false;
    }

private:
    TRefLinkedOwnershipPolicy* _next;
    TRefLinkedOwnershipPolicy* _prev;
};



class Foo {
public:
    int x;
    char y;
    int z;
    int w;

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

    {
        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TRefCountOwnershipPolicy
        > spFoo(new Foo(*sp + 200, 80));

        std::cout << "TRefCountOwnershipPolicy" << std::endl;
        std::cout << "foo = {" << spFoo->x << ", '" << spFoo->y << "'}\n";

        {
            TSmartPointer<Foo, TDefaultStoragePolicy, TRefCountOwnershipPolicy>
                    spBar(spFoo);
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
        }

        {
            TSmartPointer<
                    Foo,
                    TDefaultStoragePolicy,
                    TRefCountOwnershipPolicy
            > spBar(new Foo(1, 90));
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
            spBar = spFoo;
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
        }

        std::cout << "foo = {" << spFoo->x << ", '" << spFoo->y << "'}\n";
    }

    {
        TSmartPointer<int> nullPointer;
        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TDestructiveCopyOwnershipPolicy
        > pQux(new Foo(666, 81));

        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TDestructiveCopyOwnershipPolicy
        > pBar(pQux);

        std::cout << GetImpl(pQux) << std::endl;
        std::cout << pBar->x << " " << pBar->y << std::endl;

        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TDestructiveCopyOwnershipPolicy
        > pFoo;

        pFoo = pBar;

        std::cout << GetImpl(pBar) << std::endl;
        std::cout << pFoo->x << " " << pFoo->y << std::endl;
    }

    {
        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TRefLinkedOwnershipPolicy
        > spFoo(new Foo(*sp + 200, 80));

        std::cout << "TRefCountOwnershipPolicy" << std::endl;
        std::cout << "foo = {" << spFoo->x << ", '" << spFoo->y << "'}\n";

        {
            TSmartPointer<
                    Foo,
                    TDefaultStoragePolicy,
                    TRefLinkedOwnershipPolicy
            > spBar(spFoo);
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
        }

        {
            TSmartPointer<
                    Foo,
                    TDefaultStoragePolicy,
                    TRefLinkedOwnershipPolicy
            > spBar(new Foo(1, 90));
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
            spBar = spFoo;
            std::cout << "bar = {" << spBar->x << ", '" << spBar->y << "'}\n";
        }

        std::cout << "foo = {" << spFoo->x << ", '" << spFoo->y << "'}\n";
    }
    return 0;
}
