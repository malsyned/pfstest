from unittest import TestCase

from compat import StringIO

from pycparser.plyparser import ParseError

from automock import report_parse_error

class CLITests(TestCase):
    def test_shouldReportParseErrors(self):
        output = StringIO()

        ex = ParseError('the message')

        report_parse_error(output, 'myprog', ex)

        self.assertEqual(output.getvalue(),
                         'myprog: parse error: the message\n')
