#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
    TODO: brief

    Copyright 2013 Aman Orazaev
    Mail me aorazaev[at]gmail.com
"""

import sys
import imaplib
import email


class MailServer(object):
    """Serve email address."""

    def __init__(self, config='./mes.yaml'):
        """TODO"""
        self.load_config()

    def load_config(self):
        """Initialization from configuration file."""
        pass

    def connect(self, address):
        """(MailServer, str) -> NoneType"""
        pass

    def login(self, login, passwd):
        """(MailServer, str, str) -> NoneType"""
        pass

if __name__ == '__main__':
    import doctest
    doctest.testmod()
