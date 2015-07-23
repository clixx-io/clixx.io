import sys, platform
if sys.version_info < (2, 5):
    print(sys.stderr, "{}: need Python 2.5 or later.".format(sys.argv[0]))
    print(sys.stderror, "Your python is {}".format(sys.version))
    sys.exit(1)

from setuptools import setup

clixx_scripts = ['source/tools/clixx']
if platform.system() == 'Windows':
    clixx_scripts = ['source/tools/clixx.bat']
    
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
    scripts=clixx_scripts,
    test_suite = "tests.tests",
    zip_safe = False,

#    entry_points={
#        "console_scripts": ["clixx=source.python.__main__:main",
#        ]
#    },
#   package_data={'images': ['qt-gui/images*']},

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
