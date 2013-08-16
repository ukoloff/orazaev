#!/usr/bin/env python

class SortedDict(dict):
    def __str__(self):
        result = ""

        allKeys = self.keys()
        allKeys.sort()
        for k in allKeys:
            result += "{0} -> {1}\n".format(k, self[k])

        return result

if __name__ == "__main__":
    d = SortedDict()
    d['asdfsdfs'] = 21
    d['ad'] = 2
    d['zdf'] = 3
    d['bdads'] = 99
    d['gdsa'] = 123

    print d
