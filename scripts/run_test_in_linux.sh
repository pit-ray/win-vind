#!/bin/bash

cd test

BUILD_DIR="build_linux"

if [ ! -d $BUILD_DIR ]; then
    mkdir $BUILD_DIR
fi
cd $BUILD_DIR

cmake -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++-posix -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32 ..
cmake --build .
ctest .

cd ../..
