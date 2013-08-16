#!/usr/bin/env python

def gcd(x, y):
    """(int, int) -> int"""
    if x == 0:
        return y
    if x > y: return gcd(y, x)
    return gcd(y % x, x)

class Rational(object):
    def __init__(self, p, q):
        """(Rational, int, int) -> NoneType"""
        cd = gcd(p, q)
        self.p = p // cd
        self.q = q // cd

    def __str__(self):
        """(Rational) -> str

        >>> str(Rational(10, 3))
        '10/3'
        >>> str(Rational(2, 4))
        '1/2'
        """
        return "{0}/{1}".format(self.p, self.q)

    def __add__(self, other):
        """(Rational, Rational) -> Rational

        >>> str(Rational(10, 3) + Rational(5, 6))
        '25/6'
        >>> r = Rational(10, 3)
        >>> r += Rational(5, 6)
        >>> str(r)
        '25/6'
        """
        p1 = self.p * other.q
        p2 = self.q * other.p

        return Rational(p1 + p2, self.q * other.q)

    def __radd__(self, other):
        """(Rational, Rational) -> Rational"""
        return other.__add__(self)

if __name__ == "__main__":
    import doctest
    doctest.testmod()
