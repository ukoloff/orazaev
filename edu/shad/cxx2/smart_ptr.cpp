/**
 *  C++ course. Homework 1. Problem 2.
 *
 *  @brief TSmartPointer<T> class realization.
 *
 *
 *  Compilation:
 *  $ g++ smart_ptr.cpp -o smart_ptr.out
 *
 *
 *  Valgrind output:
 *  $ valgrind ./smart_ptr.out
 *  ==3487== Memcheck, a memory error detector
 *  ==3487== Copyright (C) 2002-2010, and GNU GPL'd, by Julian Seward et al.
 *  ==3487== Using Valgrind-3.6.1 and LibVEX; rerun with -h for copyright info
 *  ==3487== Command: ./smart_ptr.out
 *  ==3487==
 *  ==3487==
 *  ==3487== HEAP SUMMARY:
 *  ==3487==     in use at exit: 0 bytes in 0 blocks
 *  ==3487==   total heap usage: 19 allocs, 19 frees, 10,524 bytes allocated
 *  ==3487==
 *  ==3487== All heap blocks were freed -- no leaks are possible
 *  ==3487==
 *  ==3487== For counts of detected and suppressed errors, rerun with: -v
 *  ==3487== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 17 from 6)
 *
 *
 *  @author Aman Orazaev
 */
#include <cstdlib>
#include <iostream>



/// VERIFY macros for testing TSmartPointer.
#define VERIFY(EXPECT, HAS) \
    if (EXPECT != HAS) { \
        std::cout << "ASSERTION FAILED! line: " << __LINE__ << "\n"; \
        std::cout << "Expected = " << EXPECT << "\n"; \
        std::cout << "Has = " << HAS << "\n"; \
        std::abort(); \
    }



template <typename T>
class TDefaultStoragePolicy;

template <typename T>
class TNonCopyableOwnershipPolicy;



/// TSmartPointer
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
        GetImplRef(*this) = ownershipPolicyImpl.Clone(GetImplRef(other));
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



/**
 * TDefaultStoragePolicy.
 * @brief Storage policy for TSmartPointer.
 *
 * Has default delete operator (without bracets []).
 */
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



/**
 *  TArrayStoragePolicy.
 *  @brief Storage policy for TSmartPointer.
 *
 *  Has array delete operator (delete[]).
 *  Also has operator[] for indexing array;
 */
template <typename T>
class TArrayStoragePolicy : public TDefaultStoragePolicy<T> {
public:
    TArrayStoragePolicy() : TDefaultStoragePolicy<T>() { }
    TArrayStoragePolicy(
            const typename TDefaultStoragePolicy<T>::TStoredType& pointer)
        : TDefaultStoragePolicy<T>(pointer) { }

    T& operator[](size_t index) {
        return *(GetImpl(*this) + index);
    }

    const T& operator[](size_t index) const {
        return *(GetImpl(*this) + index);
    }

protected:
    void Destroy() { delete[] TDefaultStoragePolicy<T>::_pointer; }
};



/**
 *  TNonCopyableOwnershipPolicy.
 *  @brief Ownership policy for TSmartPointer.
 *
 *  Disallow copying.
 */
template <typename T>
class TNonCopyableOwnershipPolicy {
private:
    T Clone(const T&) const;

public:
    bool Release(const T&) {
        return true;
    }
};



/**
 *  TRefCountOwnershipPolicy.
 *  @brief Ownership policy for TSmartPointer.
 *
 *  Counting reference to pointer, delete when there
 *  are no reference has left.
 */
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



/**
 *  TDestructiveCopyOwnershipPolicy.
 *  @brief Ownership policy for TSmartPointer.
 *
 *  Set pointer to 0, when it is copyied in another place.
 */
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



/**
 *  TRefLinkedOwnershipPolicy.
 *  @brief Ownership policy for TSmartPointer.
 *
 *  Use double linked list for reference tracking.
 *  If it is just one element in linked list, we can
 *  delete pointer.
 *
 *  Addition of reference equal to addition of new element
 *  to linked list.
 */
template <typename T>
class TRefLinkedOwnershipPolicy {
public:
    TRefLinkedOwnershipPolicy()
        : _next(0)
        , _prev(0)
        , _cur(0) {
        _cur = this;
    }

    /// Need for Swap function in TSmartPointer.
    TRefLinkedOwnershipPolicy& operator=(const TRefLinkedOwnershipPolicy& other) {
        if (this == &other) {
            return *this;
        }
        _next = other._next;
        _prev = other._prev;
        _cur = this;

        if (_next)
            _next->_prev = _cur;
        if (_prev)
            _prev->_next = _cur;
    }

    T Clone(const T& val) {
        _cur->_next = this;
        _prev = _cur;
        _cur = this;

        if (_next) {
            _next->_prev = _cur;
        }
        return val;
    }

    bool Release(const T&) {
        if (_next == _prev) {
            return true;
        }

        if (_prev) {
            _prev->_next = _next;
        }
        if (_next) {
            _next->_prev = _prev;
        }
        _cur = _prev = _next = 0;
        return false;
    }

private:
    TRefLinkedOwnershipPolicy* _next;
    TRefLinkedOwnershipPolicy* _prev;
    TRefLinkedOwnershipPolicy* _cur;
};


/// Testing TSmartPointer and its policies.
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

template <
    template <typename> class TOwnershipPolicy
> void TestSmartPointer() {
    TSmartPointer<int> sp(new int(20));

    TSmartPointer<
            Foo,
            TDefaultStoragePolicy,
            TOwnershipPolicy
    > spFoo(new Foo(*sp + 200, 80));
    VERIFY(220, spFoo->x);
    VERIFY(80, spFoo->y);

    {
        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TOwnershipPolicy
        > spBar(spFoo);

        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TOwnershipPolicy
        > spQux(spFoo);

        VERIFY(220, spBar->x);
        VERIFY(80, spBar->y);
        VERIFY(220, spQux->x);
        VERIFY(80, spQux->y);
    }

    {
        TSmartPointer<
                Foo,
                TDefaultStoragePolicy,
                TOwnershipPolicy
        > spBar(new Foo(1, 90));
        VERIFY(1, spBar->x);
        VERIFY(90, spBar->y);

        spBar = spFoo;

        VERIFY(220, spFoo->x);
        VERIFY(80, spFoo->y);
        VERIFY(220, spBar->x);
        VERIFY(80, spBar->y);
    }
};

void TestDestructiveSmartPointer() {
    TSmartPointer<int> nullPointer;
    VERIFY(0, GetImpl(nullPointer));

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

    VERIFY(0, GetImpl(pQux));
    VERIFY(666, pBar->x);
    VERIFY(81, pBar->y);

    TSmartPointer<
            Foo,
            TDefaultStoragePolicy,
            TDestructiveCopyOwnershipPolicy
    > pFoo;

    pFoo = pBar;
    VERIFY(0, GetImpl(pBar));
    VERIFY(666, pFoo->x);
    VERIFY(81, pFoo->y);
}

template <
    template <typename> class TOwnershipPolicy
> void TestArrayPointer() {
    typedef TSmartPointer<
        int,
        TArrayStoragePolicy,
        TOwnershipPolicy
    > TTestSmartPointer;

    TTestSmartPointer pFoo(new int[100]);

    for (int i = 0; i < 100; ++i) {
        pFoo[i] = i + 800;
    }
    for (int i = 0; i < 100; ++i) {
        VERIFY(i + 800, pFoo[i]);
    }

    {
        TTestSmartPointer pBar(pFoo);
        for (int i = 0; i < 100; ++i) {
            VERIFY(i + 800, pFoo[i]);
        }
    }
    for (int i = 0; i < 100; ++i) {
        VERIFY(i + 800, pFoo[i]);
    }

    {
        TTestSmartPointer pBar(new int[1000]);
        pBar = pFoo;
        for (int i = 0; i < 100; ++i) {
            VERIFY(i + 800, pFoo[i]);
        }
    }
    for (int i = 0; i < 100; ++i) {
        VERIFY(i + 800, pFoo[i]);
    }

    {
        TTestSmartPointer pBar(new int[200]);
        for (int i = 0; i < 200; ++i) {
            pBar[i] = i + 1;
        }

        pFoo = pBar;
    }
    for (int i = 0; i < 200; ++i) {
        VERIFY(i + 1, pFoo[i]);
    }
}

int main() {
    TSmartPointer<Foo> foo(new Foo(87, 78));
    VERIFY(87, foo->x);
    VERIFY(78, foo->y);

    TestDestructiveSmartPointer();
    TestSmartPointer<TRefCountOwnershipPolicy>();
    TestSmartPointer<TRefLinkedOwnershipPolicy>();

    TestArrayPointer<TRefCountOwnershipPolicy>();
    TestArrayPointer<TRefLinkedOwnershipPolicy>();

    return 0;
}
