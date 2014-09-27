PS3 Autotests
=============

A repository of PlayStation 3 programs to perform tests, benchmarks and demos on real consoles and emulators.

* Provide a platform for benchmarking PS3 emulators and comparing their performance with a real PS3.
* Help people to see how to use some obscure-newly-discovered APIs and features.
* Allow PS3 emulators to avoid some regressions while performing refactorings and to have a reference while implementing APIs.

The main idea behind this is having several files per test unit:

* _file.expected_:  File with the expected output, preferably from a real PS3.
* _file.elf_:  The PS3 executable that will generate an output.
* _file.c_:  Source code of the unit test (Optional).

### Building
The tests can be compiled both with unofficial and official SDKs althought [PSL1GHT](http://psl1ght.com/) is recommended. For now, the tests are distributed as Visual Studio 2010 projects, you need to configure a Platform named `PS3` and the PlatformToolset named `GCC`, which should support the `FSELFFile` flag. Specific tutorials about how to do this with *PSL1GHT* will be given in the future.

### Disclaimer
The goal of this project is to experiment, research, and educate on the topic of emulation of modern devices and operating systems. **It is not for enabling illegal activity**. All information is obtained via reverse engineering of legally purchased devices and games and information made public on the Internet.
