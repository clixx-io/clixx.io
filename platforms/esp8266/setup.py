import sys
if sys.version_info < (2, 5):
    print(sys.stderr, "{}: need Python 2.5 or later.".format(sys.argv[0]))
    print(sys.stderror, "Your python is {}".format(sys.version))
    sys.exit(1)

from setuptools import setup

setup(
    name = "clixx_esp8266",
    version = "0.1.0",
    url = "http://github.com/clixx-io/clixx.io",
    license = "BSD",
    description = "Python tools system for the esp8266",
    author = "David Lyon",
    author_email = "david.lyon@clixx.io",
    # This needs to be blank
    scripts=['tools/luatool.py','tools/esptool.py','tools/terminal-esp8266/terminal-esp8266.py'],
    package_data={'terminal-esp8266': ['tools/terminal-esp8266/*']},
    test_suite = "tests.tests",
    zip_safe = False,
    install_requires = ['setuptools','paho-mqtt'],
    classifiers = [
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Topic :: System :: Logging',
    ]
)
