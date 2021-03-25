cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/dev)
add_library(dev STATIC
    mybindings.cpp
)
