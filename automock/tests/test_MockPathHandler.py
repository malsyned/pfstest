from unittest import TestCase

from automock import MockPathHandler

from os import path
class MockPathHandlerTests(TestCase):
    def setUp(self):
        self.maxDiff = None

    def test_souldStoreHeaderPath(self):
        # Given
        mpath = MockPathHandler("mockable.h", "mock-mockable")
        # When
        hpath = mpath.headerpath
        # Then
        self.assertEqual(hpath, "mockable.h")

    def test_shouldGenerateMockHeaderPath(self):
        # Given
        mpath = MockPathHandler("mockable.h", "mock-mockable")
        # When
        hpath = mpath.mockheaderpath
        # Then
        self.assertEqual(hpath, "mock-mockable.h")

    def test_shouldGenerateMockImplementationPath(self):
        # Given
        mpath = MockPathHandler("mockable.h", "mock-mockable")
        # When
        hpath = mpath.mockimplementationpath
        # Then
        self.assertEqual(hpath, "mock-mockable.c")

    def test_shouldGenerateMockHeaderPathRelativeToMockImplementationPath(self):
        # Given
        mpath = MockPathHandler("mockable.h",
                                path.join("mocks", "mock-mockable"))
        # When
        hrpath = mpath.mockheaderrelpath
        # Then
        self.assertEqual(hrpath, "mock-mockable.h")

    def test_shouldGenerateHeaderPathRelativeToMockPath(self):
        # Given
        mpath = MockPathHandler(path.join("src", "mockable.h"),
                               path.join("mocks", "mock-mockable"))
        # When
        hpath = mpath.headerrelpath
        # Then
        self.assertEqual(hpath, path.join("..", "src", "mockable.h"))

    def test_shouldGenerateHeaderPathRelativeToMockPathInSameDir(self):
        # Given
        mpath = MockPathHandler("mockable.h", "mock-mockable")
        # When
        hpath = mpath.headerrelpath
        # Then
        self.assertEqual(hpath, "mockable.h")

    def test_shouldCreateGuardMacro(self):
        # Given
        mpath = MockPathHandler("mockable.h", "mock-mockable")
        # When
        gm = mpath.guardmacro
        # Then
        self.assertEqual(gm, "MOCK_MOCKABLE_H")

    def test_shouldStripDirNameFromGuardMacro(self):
        # Given
        outputpath = path.join("..", "foo", "mock-theheader")
        mpath = MockPathHandler("mockable.h", outputpath)
        # When
        gm = mpath.guardmacro
        # Then
        self.assertEqual(gm, "MOCK_THEHEADER_H")

    def test_shouldCleanNonIdentifierCharactersFromGuardMacro(self):
        # Given
        outputpath = "some header-file?with&bad\"chars'"
        mpath = MockPathHandler("mockable.h", outputpath)
        # When
        gm = mpath.guardmacro
        # Then
        self.assertEqual(gm,
                         "SOME_HEADER_FILE_WITH_BAD_CHARS__H")
