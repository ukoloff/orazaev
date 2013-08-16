#!/usr/bin/env python

import sys

def ProcessInput():
    """ProcessInput() -> map"""
    d = {}
    for s in sys.stdin:
        key = s.split(':')[0].strip()
        values = s.split(':')[1].strip().split('/')
        for value in values:
            try:
                d[value].extend([key])
            except KeyError:
                d[value] = [key]

    return d

result = ProcessInput()

resultStrings = []

for k, vlist in result.items():
    resultStrings.append("%s: " %(k, ))
    for value in sorted(vlist):
        resultStrings[-1] += "%s, " %(value)
    resultStrings[-1] = resultStrings[-1][:-2]

for s in sorted(resultStrings):
    print s
