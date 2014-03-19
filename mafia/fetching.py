#!/usr/bin/env python

import sys

from mafia import MafiaGameRecord

if __name__ == "__main__":
    gids = sys.stdin
    if len(sys.argv) > 2:
        start = int(sys.argv[1])
        end = int(sys.argv[2]) + 1
        gids = xrange(start, end)

    print MafiaGameRecord.csvHeader()
    for gid in gids:
        gid = int(str(gid).strip())
        print >> sys.stderr, "Fetching {}".format(gid)
        try:
            print >> sys.stdout, MafiaGameRecord(gid).exportToCsv()
        except IOError:
            print >> sys.stderr, "Error to parse game {0}".format(gid)
