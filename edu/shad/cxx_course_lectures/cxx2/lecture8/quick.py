#!/usr/bin/env python
import sys

def QuickSort(l):
    """QuickSort(l) -> list"""
    if len(l) <= 1 or not l:
        return l

    i = 0
    j = len(l) - 1

    x = l[(j - i) / 2]
    while i < j:
        while l[i] < x: i += 1
        while l[j] > x: j -= 1

        if i <= j:
            t = l[i]
            l[i] = l[j]
            l[j] = t
            i += 1
            j -= 1

    if i < (len(l) - 1):
        l[:i] = QuickSort(l[:i])
    if j > 0:
        l[j:] = QuickSort(l[j:])

    return l

def qsort(L):
    if L:
        return qsort([x for x in L[1:] if x<L[0]]) + L[0:1] + qsort([x for x in L[1:] if x>=L[0]])
    return []

print qsort([5,4,23,22,30])
