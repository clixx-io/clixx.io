import sys
if sys.version_info < (2, 5):
    print(sys.stderr, "{}: need Python 2.5 or later.".format(sys.argv[0]))
    print(sys.stderror, "Your python is {}".format(sys.version))
    sys.exit(1)

from setuptools import setup

setup(
    name = "clixx_IO",
    version = "0.1.0",
    url = "http://github.com/clixx-io/clixx.io",
    license = "BSD",
    description = "A python library for clixx.io peripherals",
    author = "David Lyon",
    author_email = "david.lyon@clixx.io",
    # This needs to be blank
    packages = [''],
    package_dir = {'': 'source/python'},
    scripts=['source/tools/clixx','source/qt-gui/iot-controller.py'],
# Unknown options
#   use_2to3 = True,
#   package_data={'images': ['qt-gui/images*']},
    test_suite = "tests.tests",
    zip_safe = False,
    install_requires = ['setuptools','mako','flask','circuits','paho-mqtt'],
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
