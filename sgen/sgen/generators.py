#!/usr/bine/env python
"""
Code generators definitions for languages:
    C++
    Python

Template type names for variables should be:
    - i  : int,
    - f  : float,
    - d  : double,
    - c  : char
    - st : size_t
    - ss : std::string<>,
    - sl : std::list<>,
    - sv : std::vector<>,
    - sm : std::map<>
    - sst : std::stack<>
    - tpl : touple (For python), in c++ interped like sv.
or custom user strings.
"""

import blocks
import handlers


class CodeGenerator(object):
    """Common interface for code generators."""

    _typeDict = {}
    handlerPack = {}

    def __init__(self, tabstop=4):
        """(CodeGenerator, tabstop=4) -> NoneType"""

        self.tabstop = tabstop

    def generate(self, block):
        """(CodeGenerator, SgenBlock) -> str"""

        blockHandler = self.handlerPack[block.macro]

        if not block.blocks:
            return blockHandler(block.data, tabstop=self.tabstop, typeMap=self._typeDict)

        inside = ''
        for b in block.blocks:
            inside += self.generate(b) + '\n'

        return blockHandler(block.data, inside=inside.rstrip(),
                            tabstop=self.tabstop, typeMap=self._typeDict)


class CxxCodeGenerator(CodeGenerator):
    """C++ code generator from SgenBlocks."""

    _typeDict = {
        "i" : "int",
        "f" : "float",
        "d" : "double",
        "c" : "char",
        "st" : "size_t",
        "ss" : "std::string",
        "sl" : "std::list<>",
        "sv" : "std::vector<>",
        "sm" : "std::map<>",
        "sst" : "std::stack<>",
        "tpl" : "std::vector<>"
    }

    handlerPack = handlers.cxxHandlerPack

    def __init__(self, tabstop=4):
        """(CxxCodeGenerator, tabstop=4) -> NoneType"""

        super(CxxCodeGenerator, self).__init__(tabstop=tabstop)



class PythonCodeGenerator(CodeGenerator):
    """Python code generator from SgenBlocks."""

    _typeDict = {
        "i" : "0",
        "f" : "0.",
        "d" : "0.",
        "c" : "''",
        "st" : "0",
        "ss" : "''",
        "sl" : "[]",
        "sv" : "[]",
        "sm" : "{}",
        "sst" : "[]",
        "tpl" : "()"
    }

    handlerPack = handlers.pythonHandlerPack

    def __init__(self, tabstop=4):
        """(PythonCodeGenerator, tabstop=4) -> NoneType"""

        super(PythonCodeGenerator, self).__init__(tabstop=tabstop)


fabric = {
    'c++' : CxxCodeGenerator,
    'cxx' : CxxCodeGenerator,
    'python' : PythonCodeGenerator,
    'py' : PythonCodeGenerator
}
