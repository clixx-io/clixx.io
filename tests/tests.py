
import sys
sys.path.append('../src')

import unittest


# Here's our "unit tests".
class LineModeTests(unittest.TestCase):

    def testSingleLineComment(self):

        s = "// here is a comment"
        self.failUnless("" == "")


if __name__ == '__main__':
    unittest.main()
