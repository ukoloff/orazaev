#!/usr/bin/env python

def GetNgrammMap(s, n):
    """GetNgrammMap(s, n) -> map

    s -- input string.
    n -- n-gramm parameter.
    """
    if len(s) < n or n <= 0:
        return {}

    ngrammMap = {}
    for i in xrange(len(s) - n + 1):
        curNgramm = s[i:i + n]
        try:
            ngrammMap[curNgramm] += 1
        except KeyError:
            ngrammMap[curNgramm] = 1

    return ngrammMap


import sys

inputString = sys.argv[1]
n = int(sys.argv[2])

s = GetNgrammMap(inputString, n).items()
s.sort(lambda x, y: cmp(x[1], y[1]), reverse=True)

for k, v in s:
    print k, v
