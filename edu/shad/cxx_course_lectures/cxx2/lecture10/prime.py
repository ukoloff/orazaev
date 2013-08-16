#!/usr/bin/env python

def isPrime(p):
    """(int) -> bool

    Returns true if p is prime.
    >>> isPrime(1)
    False
    >>> isPrime(2)
    True
    >>> isPrime(23)
    True
    >>> isPrime(21)
    False
    """

    if p == 1: return False

    for d in xrange(2, p):
        if p % d == 0:
            return False

    return True



def getPrimeGenerator(p):
    """(int) -> generator of int

    >>> g = getPrimeGenerator(10)
    >>> g.next()
    11
    >>> g.next()
    13
    >>> g.next()
    17
    >>> getPrimeGenerator(2).next()
    2
    """

    while True:
        if isPrime(p):
            yield p
        p = p + 1


if __name__ == '__main__':
    import doctest
    doctest.testmod()
