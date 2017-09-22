from unittest import TestCase

from compat import StringIO
from compat import MagicMock

from automock import MockImplementationWriter
from automock import MockInfo, ReturnHint
from automock import ArgInfo, ArgHint

boilerplate_includes = """
#include <stddef.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"

"""

class MockImplementationWriterTests(TestCase):
    def setUp(self):
        self.maxDiff = None
        self.cBuffer = StringIO()

    def test_shouldWriteImplementation(self):
        # Given
        mockgen = MagicMock()
        mockgen.mockheadername = "mock-mockable.h"
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
void func1(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL);

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldWriteMultipleFunctions(self):
        # Given
        mockgen = MagicMock()
        mockgen.mockheadername = "mock-mockable.h"
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = []),
            MockInfo(mockname = "mock_func2",
                     funcname = "func2",
                     prototype = "void func2(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = []),
        ]
        mock_c_writer = MockImplementationWriter(mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
void func1(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL);

    (void)__pfstest_return_value;
}

pfstest_mock_define(mock_func2, "func2", 0);
void func2(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func2,
                            NULL);

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldHandlePrimitiveReturnTypes(self):
        # Given
        mockgen = MagicMock()
        mockgen.mockheadername = "mock-mockable.h"
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "int func1(void)",
                     return_text = "int",
                     return_hint = ReturnHint.PRIMITIVE,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
int func1(void)
{
    int __pfstest_default_return = 0;

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            the_pointer(&__pfstest_default_return));

    return *(int *)__pfstest_return_value;
}

"""
        )
