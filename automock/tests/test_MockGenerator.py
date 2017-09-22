from unittest import TestCase

from pycparser.c_parser import CParser
from pycparser.c_generator import CGenerator

from automock import MockGenerator
from automock import MockInfo, ReturnHint
from automock import ArgInfo, ArgHint

cparser = CParser()
cgen = CGenerator()
emptyast = cparser.parse('')

class MockGeneratorTests(TestCase):
    def test_souldStoreHeaderName(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        hname = mgen.headername
        # Then
        self.assertEqual(hname, "mockable.h")

    def test_shouldCreateGuardMacro(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        gm = mgen.guardmacro
        # Then
        self.assertEqual(gm, "_PFSTEST_MOCK_MOCKABLE_H")

    def test_shouldCreateMockHeaderName(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        mockh = mgen.mockheadername
        # Then
        self.assertEqual(mockh, "mock-mockable.h")

    def test_shouldGenerateMockFromOtherwiseEmptyHeader(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "void func1(void)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [])
                         ])
