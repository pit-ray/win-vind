#!/bin/bash

# Assumed to be run on MSYS2

if [ ! -e cov_tools ] ; then
    wget https://scan.coverity.com/download/cxx/win64 --post-data "token=%1&project=pit-ray%%2Fwin-vind" -O cov_tools.zip
    unzip -jv cov_tools.zip -d cov_tools 2>/dev/null
fi
