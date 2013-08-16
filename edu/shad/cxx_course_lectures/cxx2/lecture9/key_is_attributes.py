#!/usr/bin/env python

class MyDict(dict):
    def __getattribute__(self, name):
        """(MyDict, str) -> attribute

        >>> d = MyDict()
        >>> d['value'] = 12
        >>> d.value
        12
        """

        return self[name]

if __name__ == "__main__":
    import doctest
    doctest.testmod()
