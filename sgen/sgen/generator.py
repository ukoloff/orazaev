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


class CodeGenerator(object):
    """Common interface for code generators."""

    def __init__(self, tabstop=4):
        """(CodeGenerator, tabstop=4) -> NoneType"""

        self.tabstop = tabstop

    def generate(self, block):
        """(CodeGenerator, SgenBlock) -> str"""

        raise Exception("Undefined method 'generate' for CodeGenerator interface.")


class CxxCodeGenerator(CodeGenerator):
    """C++ code generator from SgenBlocks."""

    _typeDict = {
        "i" : "int",
        "f" : "float"
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

    def __init__(self, tabstop=4):
        """(CxxCodeGenerator, tabstop=4) -> NoneType"""

        super(CodeGenerator, self).__init__(tabstop=tabstop)

    def generate(self, block):
        """(CxxCodeGenerator, SgenBlock) -> str"""

        return ""


class PythonCodeGenerator(CodeGenerator):
    """Python code generator from SgenBlocks."""

    _typeDict = {
        "i" : "= 0",
        "f" : "= 0.",
        "d" : "= 0.",
        "c" : "= ''",
        "st" : "= 0",
        "ss" : "= ''",
        "sl" : "= []",
        "sv" : "= []",
        "sm" : "= {}",
        "sst" : "= []",
        "tpl" : "= ()"
    }

    def __init__(self, tabstop=4)
        """(PythonCodeGenerator, tabstop=4) -> NoneType"""

        super(CodeGenerator, self).__init__(tabstop=tabstop)
