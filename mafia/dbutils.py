#!/usr/bin/env python
# -*- coding: utf-8 -*-

import bsddb


class HashDb(object):
    def __init__(self, pathToDb):
        self._db = bsddb.hashopen(pathToDb)

    def __enter__(self):
        return self._db

    def __exit__(self, type, value, tb):
        self._db.close()

