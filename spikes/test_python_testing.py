from unittest import TestCase

class ExperimentWithTests(TestCase):
    def test_shouldPass(self):
        i = True
        self.assertEqual(i, True)
        
    def test_shouldFail(self):
        i = True
        self.assertEqual(i, False)

    def test_shouldHandleLists(self):
        l = [1, 2, 3]
        self.assertSequenceEqual(l, [1, 2, 4])
        
