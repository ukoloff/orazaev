#!/usr/bin/env python
"""TODO"""


import re
import blocks

RE_MACRO = re.compile(r'(?P<name>%[^\s\(]*)(?P<arg>\([^\)]*\))?')


class UnknownMacroError(Exception):
    pass

class UnclosedMacroError(Exception):
    pass


class BlockParser(object):
    """TODO"""

    def __init__(self):
        """(BlockParser) -> NoneType"""

        pass

    @staticmethod
    def parseMacro(macroName, macroArg, lineno=None):
        """TODO"""

        if macroName not in blocks.fabric.keys():
            raise UnknownMacroError("Unknown macro '{0}' at line {1}"
                                    .format(macroName, lineno))

        macroData = macroArg[1: -1] if macroArg is not None else None
        return blocks.fabric[macroName](data=macroData)


    def parse(self, data):
        """(BlockParser, str) -> SgenBlock"""

        stack = [blocks.SourceFileBlock()]

        lineno = 0
        for line in data.splitlines():
            lineno += 1
            line = line.strip()

            macro = re.match(RE_MACRO, line)

            if macro:
                if macro.group('name') == blocks.END:
                    del stack[-1]
                    continue

                block = self.parseMacro(macro.group('name'),
                                        macro.group('arg'),
                                        lineno=lineno)

                stack[-1].blocks.append(block)
                if block.isOpened():
                    stack.append(block)
                continue

            # Add raw text to current block by default
            stack[-1].blocks.append(blocks.RawTextBlock(data=line))

        if len(stack) != 1:
            raise UnclosedMacroError(
                'There are unclosed macro in template description, stack: {0}'
                .format(stack))

        stack[0].close()
        return stack[0]
