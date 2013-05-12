#!/usr/bin/env python

import sys

from mafia import MafiaGameRecord

if __name__ == "__main__":

    print MafiaGameRecord.csvHeader()
    for gid in sys.stdin:
        gid = int(gid.strip())
        try:
            print MafiaGameRecord(gid).exportToCsv()
        except:
            print >> sys.stderr, "Error to parse game {0}".format(gid)
