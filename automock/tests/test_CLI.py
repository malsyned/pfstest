# SPDX-License-Identifier: LGPL-2.1-or-later

from unittest import TestCase

from os import path, pardir

from compat import StringIO
from pycparser.plyparser import ParseError

from automock import report_parse_error

class CLITests(TestCase):
    def test_shouldReportParseErrors(self):
        output = StringIO()

        report_parse_error(output, path.join(pardir, 'foo', 'myprog.py'),
                           ParseError('the message'))

        self.assertEqual(output.getvalue(),
                         'myprog: parse error: the message\n')
