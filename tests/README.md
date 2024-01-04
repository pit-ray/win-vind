# win-vind Test
**In this documents, we assume executing commands in tests directory not in the project root.**

## Unit Test
It are run using CTest at compile time. This is based on branch coverage.

### Visual Studio 2019
```bash
$ cmake -B build_msvc -G "Visual Studio 16 2019" unit
$ cmake --build build_msvc
$ ctest -C Debug --test-dir build_msvc --output-on-failure
```

### MinGW-w64 >= GCC 11.2.0
```bash
$ cmake -B build_mingw -G "MinGW Makefiles" unit
$ cmake --build build_mingw
$ ctest -C Debug --test-dir build_mingw --output-on-failure
```

## Runtime Test
This tool uses to test for integration.
Specify the path of executable file after built win-vind, and then this tools generate proper keystrokes to check the win-vind behavior. However, It enumlates user inputs actually, so may destroy other applications that are currently running. For this, it should be run in a virtual environment such as VirtualBox.

### Requirements
The runtime test is implemented in python scripts.


### Run Test
```bash
$ python runtime/test.py "../bin_64/win-vind/win-vind.exe"
```
