cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/mode)
add_library(mode STATIC
    change_mode.cpp
    command_mode.cpp
)
enable_cotire(mode)
