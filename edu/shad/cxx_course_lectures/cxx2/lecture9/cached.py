#!/usr/bin/env python

cached_dict = {}

def cached(f):
    """(func) -> func"""
    try:
        cached_dict[f]
    except KeyError:
        cached_dict[f] = {}

    def my_f(x):
        try:
            print "Has cached: {0}".format(cached_dict[f][x])
            return cached_dict[f][x]
        except KeyError:
            cached_dict[f][x] = f(x)

        return cached_dict[f][x]

    return my_f

@cached
def g(x):
    return x + 1

if __name__ == "__main__":
    for x in xrange(10):
        print g(1)
    print g(2)
    print g(3)
    print g(4)
    print g(2)
