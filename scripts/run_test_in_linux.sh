#!/bin/bash

cd test

cmake -B build_linux -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32 .
cmake --build build_linux

cd build_linux
ctest

cd ../..
