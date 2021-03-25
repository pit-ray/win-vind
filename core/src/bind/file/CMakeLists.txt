cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/file)
add_library(file STATIC
    filer.cpp
    mkdir.cpp
)
enable_cotire(file)

