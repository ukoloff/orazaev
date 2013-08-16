#!/usr/bin/env python
# -*- coding: utf-8 -*-


import sys
import optparse

def main():
    """() -> NoneType Ф"""

    parser = optparse.OptionParser()
    parser.add_option('-L', dest='longestSize', nargs=0, help='Print longest string size in file.')
    parser.add_option('-c', dest='byteCount', nargs=0, help='Print number of bytes.')
    parser.add_option('-l', dest='linesCount', nargs=0, help='Print number of lines.')
    parser.add_option('-m', dest='charCount', nargs=0, help='Print number of characters.')
    parser.add_option('-w', dest='wordsCount', nargs=0, help='Print number of words.')

    options, files = parser.parse_args()

    print options
    if not options.linesCount is None:
        print 'linesCount'

    print files

    for next_file in files:
        with open(next_file, 'rb') as cur_file:
            lineCount = 0
            byteSize = 0
            longestSize = 0
            wordCount = 0
            charCount = 0
            for line in cur_file.xreadlines():
                lineCount += 1
                byteSize += len(line)
                wordCount += len(line.split())
                charCount += len(line.decode('utf-8'))
                if len(line) > longestSize:
                    longestSize = len(line)

            if not options.linesCount is None:
                print 'Lines:', lineCount

            if not options.byteCount is None:
                print 'Bytes:', byteSize

            if not options.longestSize is None:
                print 'Max line:', longestSize

            if not options.charCount is None:
                print 'Chars:', charCount

            if not options.wordsCount is None:
                print 'Words', wordCount


if __name__ == '__main__':
    main()
