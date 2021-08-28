#!/bin/bash

# Assumed to be run on MSYS2
if [! -e bin]; then
    mkdir bin
    cd bin
    wget https://scan.coverity.com/download/cxx/win64 --post-data "token=$TOKEN&project=pit-ray%2Fwin-vind" -O coverity_tool.zip
    unzip coverity_tool.zip
    cd ..
fi
