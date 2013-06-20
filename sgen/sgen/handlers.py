#!/usr/bin/env python
"""
Handler packs for blocks definitions.
"""

import textwrap

import blocks


def dummyHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       Dummy block handler.
       Do nothing, just return what is inside the block.
    """

    return inside


def textHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       returns data.
    """

    return data


def makeIndent(text, tabstop=4):
    """(str, tabstop=4) -> str

       add indentation for text.
    """

    result = ''
    for line in text.splitlines():
            result += ((' ' * tabstop + line) if line else '') + '\n'

    return result


######################################
# See C++ handlers defenitions below #
######################################


def cxxSourceHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str"""

    comment = textwrap.dedent('''\
        /**
        {0}TODO: brief

        {0}Copyright 2013 Aman Orazaev
        {0}Mail me aorazaev[at]gmail.com
        */
        
        '''.format(' ' * tabstop))

    return comment + inside


def cxxCommentHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> cxxCommentHandler(None, inside='Test Comment\\n\\nCopyright 2013 John Smith.')
       '/*\\n    Test Comment\\n\\n    Copyright 2013 John Smith.\\n*/'
    """

    return '/*\n{0}*/'.format(makeIndent(inside, tabstop=tabstop))


def cxxFunctionHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> cxxFunctionHandler('d pi', inside='return 3.14;', typeMap={'d':'double'})
       'double pi() {\\n    return 3.14;\\n}'
       >>> cxxFunctionHandler('double e', inside='return 2.71;', tabstop=2)
       'double e() {\\n  return 2.71;\\n}'
    """

    args = data.split()
    assert len(args) > 0, '{0} macro must have 1 or more arguments'.format(blocks.FUNCTION)

    funcType = args[0]
    funcName = args[0]
    funcFormat = '{0} {1}'

    if len(args) == 1:
        funcFormat = '{1}'
    else:
        funcName = args[1]
        if funcType in typeMap.keys():
            funcType = typeMap[funcType]

    return funcFormat.format(funcType, funcName) + '() {\n' + makeIndent(inside, tabstop) + '}'


def cxxVariableHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> cxxVariableHandler('int x')
       'int x;'
       >>> cxxVariableHandler('d pi', typeMap={'d':'double'})
       'double pi;'
    """

    args = data.split()
    assert len(args) >= 2, '{0} macro must have 2 or more arguments'.format(blocks.VARIABLE)

    varType = typeMap[args[0]] if args[0] in typeMap.keys() else args[0]
    varName = args[1]

    return "{0} {1};".format(varType, varName)


def cxxClassHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> cxxClassHandler('TestClass')
       '/*\\n    TODO: brief class description\\n*/\\nclass TestClass {\\n};'
       >>> cxxClassHandler('TTest', inside='TTest TTest() {\\n  printf("Constructor");\\n}', tabstop=2)
       '/*\\n  TODO: brief class description\\n*/\\nclass TTest {\\n  TTest TTest() {\\n    printf("Constructor");\\n  }\\n};'
    """

    def modificatorsIndent(text):
        result = ''
        for line in text.splitlines():
            if line.strip() in ['public:', 'private:', 'protected:']:
                result += line[tabstop:] + '\n'
            else:
                result += line + '\n'

        return result

    comment = '/*\n' + ' ' * tabstop + 'TODO: brief class description\n*/\n'
    body = 'class {0} '.format(data) + '{\n' + modificatorsIndent(makeIndent(inside, tabstop)) + '};'
    return comment + body


cxxHandlerPack = {
    blocks.SourceFileBlock.macro : cxxSourceHandler,
    blocks.CommentBlock.macro : cxxCommentHandler,
    blocks.RawTextBlock.macro : textHandler,
    blocks.FunctionBlock.macro : cxxFunctionHandler,
    blocks.VarBlock.macro : cxxVariableHandler,
    blocks.CopyrightBlock.macro : cxxCommentHandler,
    blocks.ClassBlock.macro : cxxClassHandler
}


#########################################
# See Python handlers description below #
#########################################


def pythonSourceFileHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str"""

    comment = textwrap.dedent('''\
        #!/usr/bin/env python
        # -*- coding: utf-8 -*-
        """
        {0}TODO: brief

        {0}Copyright 2013 Aman Orazaev
        {0}Mail me aorazaev[at]gmail.com
        """
        
        '''.format(' ' * tabstop))

    epilog = "if __name__ == '__main__':\n"
    epilog += makeIndent('import doctest\ndoctest.testmod()', tabstop=tabstop)

    return comment + inside + '\n\n\n' + epilog.rstrip()


def pythonCommentHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> pythonCommentHandler(None, inside='This is multiline\\n\\nComment.')
       '# This is multiline\\n#\\n# Comment.'
    """

    return "\n".join(('# ' if l else '#') + l for l in inside.splitlines())


def pythonFunctionHandler(data, inside='', tabstop=4, typeMap={}):
    '''(str, inside='', tabstop=4, typeMap={}) -> str

       >>> pythonFunctionHandler('i main')
       'def main():\\n    """TODO"""\\n\\n    pass'
       >>> pythonFunctionHandler('getCurrentTime', inside='from time import time\\nreturn time()', tabstop=2)
       'def getCurrentTime():\\n  """TODO"""\\n\\n  from time import time\\n  return time()'
    '''

    funcName = data.split()[-1]

    if not inside.strip():
        inside = 'pass'
    inside = '"""TODO"""\n\n' + inside

    return 'def {0}():\n'.format(funcName) + makeIndent(inside, tabstop=tabstop)[:-1]


def pythonVariableHandler(data, inside='', tabstop=4, typeMap={}):
    """(str, inside='', tabstop=4, typeMap={}) -> str

       >>> pythonVariableHandler('l stack', typeMap={'l':'[]'})
       'stack = []'
       >>> pythonVariableHandler('() x')
       'x = ()'
    """

    args = data.split()
    assert len(args) >= 2, "{0} macro must have 2 or more arguments".format(blocks.VARIABLE)

    varName = args[1]
    varValue = typeMap[args[0]] if args[0] in typeMap.keys() else args[0]

    return "{0} = {1}".format(varName, varValue)


def pythonClassHandler(data, inside='', tabstop=4, typeMap={}):
    '''(str, inside='', tabstop=4, typeMap={}) -> str

       >>> pythonClassHandler('TestClass')
       'class TestClass(object):\\n    """TODO"""\\n\\n    pass'
       >>> pythonClassHandler('MyClass', inside='x = 0\\n\\ndef f():\\n  """TODO"""\\n  pass', tabstop=2)
       'class MyClass(object):\\n  """TODO"""\\n\\n  x = 0\\n\\n  def f():\\n    """TODO"""\\n    pass'
    '''

    className = data.split()[0]

    if not inside:
        inside = 'pass'
    inside = '"""TODO"""\n\n' + inside

    return 'class {0}(object):\n'.format(className) + makeIndent(inside, tabstop=tabstop)[:-1]


pythonHandlerPack = {
    blocks.SourceFileBlock.macro : pythonSourceFileHandler,
    blocks.CommentBlock.macro : pythonCommentHandler,
    blocks.RawTextBlock.macro : textHandler,
    blocks.FunctionBlock.macro : pythonFunctionHandler,
    blocks.VarBlock.macro : pythonVariableHandler,
    blocks.CopyrightBlock.macro : pythonCommentHandler,
    blocks.ClassBlock.macro : pythonClassHandler
}


if __name__ == '__main__':
    import doctest
    doctest.testmod()
