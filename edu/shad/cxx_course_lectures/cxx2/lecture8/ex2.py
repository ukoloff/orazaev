#!/usr/bin/env python

import sys
N = int(sys.argv[1])


def CalcPrimes(N):
    """CalcPrimes(N) -> list"""
    if N < 2:
        return []

    primes = []
    for number in xrange(2, N):
        for prime in primes:
            if number % prime == 0:
                break
        else:
            primes.append(number)

    return primes

def CalcDecompositions(N):
    """CalcDecompositions(N) -> map"""
    primes = CalcPrimes(N)

    if N <= 0:
        return {}

    decompositions = {1:[]}

    for number in xrange(2, N):
        curDecomposition = []
        curNumber = number

        for prime in primes:
            power = 0
            while number % prime == 0:
                power += 1
                number /= prime

            if power:
                curDecomposition.append((prime, power))

        decompositions[curNumber] = curDecomposition

    return decompositions


#print CalcPrimes(N)
print CalcDecompositions(N)
