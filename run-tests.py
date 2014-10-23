#!/usr/bin/python

import os
import ntpath
import sys
import subprocess

# Constants
OUTPUT_STDOUT = 0 # All output is obtained through stdout
OUTPUT_FILE   = 1 # The test generates a file called 'output.txt'

COMPARE_BIN   = 0 # Output is validated byte per byte
COMPARE_TEXT  = 1 # Output is validated as text, replacing CRLF with LF
COMPARE_SMART = 2 # Output is validated following rules specified below

# Absolute path to this script
scriptFolder = os.path.dirname(os.path.realpath(__file__))

# List of tests and benchmarks
autotests = (
    ('tests/cpu/basic',                   OUTPUT_STDOUT,  COMPARE_TEXT),
    ('tests/cpu/ppu_branch',              OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_arithmetic',    OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_compare',       OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_conversion',    OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_arithmetic',  OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_compare',     OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_logical',     OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_rotate',      OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_shift',       OUTPUT_FILE,    COMPARE_TEXT),
)

# Main
def runTests(emulator, baseDir):
    errors = False

    # Run tests
    for test in autotests:
        # Parameters
        relProjectFolder = test[0]
        absProjectFolder = os.path.join(scriptFolder, relProjectFolder)
        projectName = ntpath.basename(relProjectFolder)
        elfPath = os.path.join(absProjectFolder, projectName) + '.ppu.self'
        expectedPath = os.path.join(absProjectFolder, projectName) + '.expected'
        outputPath = os.path.join(absProjectFolder, 'output.txt')
        outputMethod = test[1]
        compareMethod = test[2]

        # Command & Expected file
        cmd = emulator + ' ' + os.path.join(baseDir, elfPath)
        expected = open(os.path.join(baseDir, expectedPath), 'rb')

        # Get output
        if outputMethod == OUTPUT_STDOUT:
            result = subprocess.check_output(cmd, shell=True)
        if outputMethod == OUTPUT_FILE:
            subprocess.check_output(cmd, shell=True);
            result = open(outputPath, 'rb').read()

        # Compare output
        if compareMethod == COMPARE_TEXT:
            result = result.replace('\r\n', '\n')
            if result == expected.read():
                print " - Success:", relProjectFolder
            else:
                print " - Error:", relProjectFolder
                print result
                errors = True

        # Delete output file if necessary
        if outputMethod == OUTPUT_FILE:
            os.remove(outputPath)

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
