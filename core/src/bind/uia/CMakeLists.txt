cmake_minimum_required(VERSION 3.0.0)
include_directories(
    ${WINDOWS_SDK_INCLUDE_DIR}
    ${BIND_INCLUDE_DIR}/uia
)
add_library(uia STATIC
    easy_click.cpp
    uia.cpp
)
enable_cotire(uia)

