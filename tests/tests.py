#/usr/bin/python
import os, sys
import unittest
import random

from clixxIO import *

# Here's our "unit tests".

class EnvironmentManagementTests(unittest.TestCase):

    def testNonNullProjectDirectory(self):
        self.failUnless(clixxIOProjectDir() != "")

class ProjectManagementTests(unittest.TestCase):

    def setUp(self):
        random.random()
        self.projects = clixxIOProjectDir()

    def testAddProject(self):
        # Test creation of a project
        p = str(random.randrange(1000, 9999, 1))

        clixxIOAddProject(p)

        self.failUnless(os.path.exists(os.path.join(clixxIOProjectDir(),p)))

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
