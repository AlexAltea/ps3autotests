#!/usr/bin/python

import os
import re

# Directories
PS3AUTOTESTS_DIR = os.path.abspath("..")
PS3AUTOTESTS_BENCHMARKS = os.path.join(PS3AUTOTESTS_DIR, "benchmarks")
PS3AUTOTESTS_COMMON = os.path.join(PS3AUTOTESTS_DIR, "common")
PS3AUTOTESTS_TESTS = os.path.join(PS3AUTOTESTS_DIR, "tests")


# Formatting rules for *.c, *.cpp and *.h files
def formatGeneric(codeInput):

    # Replace LF with CRLF
    codeInput = re.sub(r'^[\r]\n', r'\r\n', codeInput)

    # Replace tabs with 4 spaces
    codeInput = re.sub(r'\t', r'    ', codeInput)

    # Remove tabs or spaces at the end of lines
    codeInput = re.sub(r'([ \t]+)\r', r'\1\r', codeInput)
    
    return codeInput


# Search and edit files of the PS3Autotests project
def main():
    for path in [PS3AUTOTESTS_BENCHMARKS, PS3AUTOTESTS_COMMON, PS3AUTOTESTS_TESTS]:
        for root, dirs, files in os.walk(path):
            for filename in files:
                if not filename.endswith((".c",".cc",".cpp",".h",".hpp")):
                    continue

                # Read and format the code
                f = open(os.path.join(root, filename), "rb")
                codeInput = f.read()
                codeOutput = formatGeneric(codeInput)
                f.close()

                # Update file if necessary
                if codeInput != codeOutput:
                    w = open(os.path.join(root, filename), "wb")
                    w.write(codeOutput)
                    w.close()
                    break


if __name__ == "__main__":
    main()
