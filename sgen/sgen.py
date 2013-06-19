#!/usr/bin/env python
"""
TODO
"""

import argparse
import textwrap
import sys

import sgen.blocks as blocks
import sgen.parser as parser
import sgen.generators as generators


def argumentsParsing():
    """TODO"""

    argparser = argparse.ArgumentParser()
    argparser.description = 'script for automatically source templates generation.'
    argparser.epilog = textwrap.dedent("""\
        input file macrocommands:
          {0}             copyright message.
          {1}                   end of block.
          {2} NAME             function NAME.
          {3} NAME            class NAME.
          {4}               commented block.
          {5} TYPE NAME         variable defenition."""
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


def main():
    """TODO"""

    args = argumentsParsing()

    prog = parser.BlockParser().parse(args.input)
    if args.lang == 'blocks':
        prog.printBlocks()
        sys.exit(0)

    if args.lang not in generators.fabric.keys():
        print >> sys.stderr, "Unknown language '{0}'".format(args.lang)
        sys.exit(1)

    generator = generators.fabric[args.lang]()
    print generator.generate(prog)


if __name__ == "__main__":
    main()
