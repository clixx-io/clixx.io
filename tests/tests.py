
import sys
import unittest

from clixxIO import *

# Here's our "unit tests".
class LineModeTests(unittest.TestCase):

    def testSingleLineComment(self):

        s = "// here is a comment"
        self.failUnless("" == "")

class I2CTests(unittest.TestCase):
    
    def AddressRead(self):
        
        r = """0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- 2d -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- 48 49 -- -- -- -- -- --
50: 50 51 52 -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- 69 -- -- -- -- -- --
70: -- -- -- -- -- -- -- --"""


if __name__ == '__main__':
    unittest.main()
