#!/usr/bin/env python
"""
TODO
"""

import argparse
import textwrap
import sys

import sgen.blocks as blocks
import sgen.parser as parser


def argumentsParsing():
    """TODO"""

    argparser = argparse.ArgumentParser()
    argparser.description = 'script for automatically source templates generation.'
    argparser.epilog = textwrap.dedent("""\
        input file macrocommands:
          {0}             copyright message.
          {1}                   end of block.
          {2}(NAME)            function NAME.
          {3}(NAME)           class NAME.
          {4}               commented block.
          {5}(TYPE NAME)        variable defenition."""
          .format(blocks.COPYRIGHT, blocks.END,
                  blocks.FUNCTION, blocks.CLASS,
                  blocks.COMMENT, blocks.VARIABLE))

    argparser.formatter_class = argparse.RawDescriptionHelpFormatter

    argparser.add_argument('-l', '--lang', metavar='LANG',
                           help='Templates programming language.(c++, python, blocks...)')
    argparser.add_argument('-i', '--input', metavar='FILE',
                           help='Input file with template description.')

    args = argparser.parse_args()

    if args.input is None:
        args.input = sys.stdin.read()
    else:
        args.input = open(args.input, 'rb').read()

    if args.lang is None:
        args.lang = 'blocks'
    else:
        args.lang = args.lang.lower().strip()

    return args


def sgen():
    """TODO"""

    args = argumentsParsing()

    prog = parser.BlockParser().parse(args.input)
    prog.printBlocks()


if __name__ == "__main__":
    sgen()
