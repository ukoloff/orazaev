#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import argparse
import logging

from mafia import MafiaGameRecord, getGameIds, ParsingError
from dbutils import HashDb


def parse_args():
    parser = argparse.ArgumentParser(
        description="Do fetching games from mafshow.ru site")
    parser.add_argument('gamesdb', help='path to games db')
    return parser.parse_args()


def main(args):
    with HashDb(args.gamesdb) as db:
        for gid in getGameIds():
            if gid in db:
                logging.info('Skipping {}'.format(gid))
                continue

            logging.info('Fetching {}'.format(gid))
            try:
                db[gid] = MafiaGameRecord(gid).exportToJson()
            except ParsingError as pe:
                logging.error(
                    'Can\'t parse game {0}, because of: {1}'.format(gid, pe))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    main(parse_args())
