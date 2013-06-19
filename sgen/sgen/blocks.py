#!/usr/bin/env python
"""
Program blocks definitions.
"""

import textwrap
from datetime import datetime


# Macro commands
COPYRIGHT = '%copyright'
COMMENT = '%comment'
FUNCTION = '%func'
CLASS = '%class'
VARIABLE = '%var'
RAW = '%raw'
END = '%end'

# Macro for internal representation
SOURCE = '%source'


class SgenBlock(object):
    """Composition of program blocks."""

    macro = None

    def __init__(self, data=None):
        """(SgenBlock, data=None) -> NoneType"""

        self.data = data
        self.blocks = []
        self.opened = True

    def isOpened(self):
        """(SgenBlock) -> bool"""

        return self.opened

    def close(self):
        """(SgenBlock) -> NoneType"""

        self.opened = False

    def printBlocks(self, indent=0):
        """(SgenBlock, indent=0) -> NoneType

           Printing block structure for debugging.
        """

        print " " * (indent * 4) + "{0}: {1}".format(self.__class__.__name__, self.data)

        for block in self.blocks:
            block.printBlocks(indent=indent+1)


class SourceFileBlock(SgenBlock):
    """Block for source file."""

    macro = SOURCE

    def __init__(self, data=None):
        """(SourceFileBlock, data=None) -> NoneType"""

        super(SourceFileBlock, self).__init__(data=data)


class CommentBlock(SgenBlock):
    """Block of comments."""

    macro = COMMENT

    def __init__(self, data=None):
        """(CommentBlock, data=None) -> NoneType"""

        super(CommentBlock, self).__init__(data=data)


class CopyrightBlock(CommentBlock):
    """Copyright comment."""

    macro = COPYRIGHT

    def __init__(self, data=None):
        """(CopyrightBlock, data=None) -> NoneType"""

        super(CopyrightBlock, self).__init__()

        data=textwrap.dedent("""\
            TODO: description

            Copyright {0} Aman Orazaev
            Mail me: aorazaev[at]gmail.com"""
            .format(datetime.now().year))

        self.blocks = [RawTextBlock(data=l) for l in data.splitlines()]
        self.close()


class FunctionBlock(SgenBlock):
    """Function definition in program."""

    macro = FUNCTION

    def __init__(self, data=None):
        """(FunctionBlock, data=None) -> NoneType"""

        super(FunctionBlock, self).__init__(data=data)


class ClassBlock(SgenBlock):
    """Class definition in program."""

    macro = CLASS

    def __init__(self, data=None):
        """(ClassBlock, data=None) -> NoneType"""

        super(ClassBlock, self).__init__(data=data)


class VarBlock(SgenBlock):
    """Variable and class fields defenitions in program."""

    macro = VARIABLE

    def __init__(self, data=None):
        """(VarBlock, data=None) -> NoneType"""

        super(VarBlock, self).__init__(data=data)
        self.close()


class RawTextBlock(SgenBlock):
    """TODO"""

    macro = RAW

    def __init__(self, data=None):
        """(RawTextBlock, data=None) -> NoneType"""

        super(RawTextBlock, self).__init__(data=data)
        self.close()


fabric = {
    COPYRIGHT : CopyrightBlock,
    COMMENT : CommentBlock,
    '%%'    : CommentBlock,
    FUNCTION : FunctionBlock,
    CLASS : ClassBlock,
    VARIABLE : VarBlock,
    RAW : RawTextBlock
}
