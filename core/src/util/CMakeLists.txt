cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/util)
add_library(util STATIC
    color.cpp
    debug.cpp
    string.cpp
    winwrap.cpp
)
enable_cotire(util)
