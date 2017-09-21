from unittest import TestCase
from unittest.mock import MagicMock

from automock import MockHeaderWriter, MockInfo

import sys
if (sys.version_info[0] >= 3): from io import StringIO
else:                          from StringIO import StringIO

class MockHeaderWriterTests(TestCase):
    def setUp(self):
        self.hBuffer = StringIO()

    def test_shouldWriteHeader(self):
        # Given
        mockgen = MagicMock()
        mockgen.guardmacro = "MOCK_MOCKABLE_H"
        mockgen.headername = "mockable.h"
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "", prototype = "", return_text = "",
                     return_hint = "", args_info = []),
            MockInfo(mockname = "mock_func2",
                     funcname = "", prototype = "", return_text = "",
                     return_hint = "", args_info = [])]
        mock_h_writer = MockHeaderWriter(mockgen)

        # When
        mock_h_writer.write_header(self.hBuffer)

        # Then
        self.assertEqual(
            self.hBuffer.getvalue(),
            """#ifndef MOCK_MOCKABLE_H
#define MOCK_MOCKABLE_H

#include "mockable.h"

#include "pfstest-mock.h"

pfstest_mock_declare(mock_func1);
pfstest_mock_declare(mock_func2);

#endif /* !defined(MOCK_MOCKABLE_H) */
"""
            )
