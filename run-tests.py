#!/usr/bin/python

import os
import sys
import subprocess

# Constants
OUTPUT_FILE   = 0 # The test generates a file called 'project.raw'
OUTPUT_STDOUT = 1 # All output is obtained through stdout

COMPARE_BIN   = 0 # Output is validated byte per byte
COMPARE_TEXT  = 1 # Output is validated as text, replacing CRLF with LF
COMPARE_SMART = 2 # Output is validated following rules specified below

# List of tests and benchmarks
autotests = (
    ('tests/cpu/basic/basic', OUTPUT_STDOUT, COMPARE_TEXT),
)

# Main
def runTests(emulator, baseDir):
    errors = False

    # Run tests
    for test in autotests:
        # Parameters
        project = test[0][:test[0].rfind('/')]
        elfPath = test[0] + '.ppu.self'
        expectedPath = test[0] + '.expected'
        outputMethod = test[1]
        compareMethod = test[2]

        # Command & Expected file
        cmd = emulator + ' ' + os.path.join(baseDir, elfPath)
        expected = open(os.path.join(baseDir, expectedPath), 'rb')

        # Get output
        if outputMethod == OUTPUT_STDOUT:
            result = subprocess.check_output(cmd, shell=True)

        # Compare output
        if compareMethod == COMPARE_TEXT:
            result = result.replace('\r\n', '\n')
            if result == expected.read():
                print " - Success:", project
            else:
                print " - Error:", project
                errors = True

    # Return errors
    if errors:
        print "Some tests failed!"
        exit(1)
    else:
        print "All tests succeeded!"
        exit(0)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print 'PS3 Autotests: Tests, benchmarks and demos for emulators'
        print 'Usage: run-tests.py [arguments] path/to/emulator.exe'
        print 'Arguments: (none available)'
    else:
        baseDir = os.path.dirname(os.path.abspath(__file__)).replace('\\', '/')
        emulator = sys.argv[len(sys.argv) - 1]
        runTests(emulator, baseDir)
