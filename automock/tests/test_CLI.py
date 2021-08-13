# SPDX-License-Identifier: LGPL-2.1-or-later

from unittest import TestCase

from os import path, pardir

from compat import StringIO
from pycparser.plyparser import ParseError

from automock import report_parse_error

class TTYStringIO(StringIO):
    """StringIO except claims to be a tty"""
    def isatty(self):
        return True

class CLITests(TestCase):
    def test_shouldReportParseErrors(self):
        output = StringIO()

        report_parse_error(output, path.join(pardir, 'foo', 'myprog.py'),
                           ParseError('the message'))

        self.assertEqual(output.getvalue(),
                         'myprog: parse error: the message\n')

    def test_shouldUseANSIColorOnTTYs(self):
        output = TTYStringIO()

        report_parse_error(output, path.join(pardir, 'foo', 'myprog.py'),
                           ParseError('the message'))

        self.assertEqual(output.getvalue(),
                         '\033[1mmyprog:\033[m \033[31;1mparse error:\033[m the message\n')
