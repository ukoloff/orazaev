#!/usr/bin/env python
"""
:file: executils.py
:brief: execution of messages utils.

Execute given code in choosen interpreter. By default
python interpreter is used::
    >>> exec_interpreter('''
    ... import sys
    ... print "Hello"
    ... print >> sys.stderr, "World"
    ... ''')
    ('Hello\\n', 'World\\n', 0)

You can choose your own inerpreter by specifying interp keyword::
    >>> exec_interpreter('''
    ... echo 'Hello'
    ... echo 'World' 1>/dev/null >&2''', interp='bash')
    ('Hello\\n', 'World\\n', 0)

If there are incorrect interpreter name is specified, then
you'll see bash error in stderr output::
    >>> exec_interpreter('asdf', interp='INCORRECT_INTERPRETER_NAME')
    ('', 'Cannot start interpreter(INCORRECT_INTERPRETER_NAME) pipe. Got exception: [Errno 2] No such file or directory', 666)
"""

import time

from subprocess import Popen
from subprocess import PIPE


def exec_interpreter(code, interp='python'):
    """(str) -> (str, str, int)

       Execute given @code using given interpreter.
       :returns: touple (stdout, strerr, return code).

       .. TODO::
          * Use non-blocking poll instead of wait.
          * Timeout killing
    """
    try:
        p = Popen((interp,), stdin=PIPE, stdout=PIPE, stderr=PIPE)
    except Exception as e:
        return ('',
                'Cannot start interpreter({0}) pipe. Got exception: {1}'.format(interp, str(e)),
                666)
    try:
        p.stdin.write(code)
    except Exception as e:
        return ('',
                'Cannot write to interpreter({0}) pipe. Got exception: {1}'.format(interp, str(e)),
                666)
    p.stdin.close()
    p.wait()
    return (p.stdout.read(), p.stderr.read(), p.returncode)


if __name__ == '__main__':
    import doctest
    doctest.testmod()
