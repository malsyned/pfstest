# SPDX-License-Identifier: LGPL-2.1-or-later

from unittest import TestCase

from compat import StringIO
from compat import MagicMock

from automock import MockHeaderWriter, MockInfo

class MockHeaderWriterTests(TestCase):
    def setUp(self):
        self.hBuffer = StringIO()

    def test_shouldWriteHeader(self):
        # Given
        mockpaths = MagicMock()
        mockpaths.headerrelpath = "mockable.h"
        mockpaths.guardmacro = "MOCK_MOCKABLE_H"
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "", prototype = "", return_text = "",
                     return_hint = "", args_info = []),
            MockInfo(mockname = "mock_func2",
                     funcname = "", prototype = "", return_text = "",
                     return_hint = "", args_info = [])]
        mock_h_writer = MockHeaderWriter(mockpaths, mockgen)

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
