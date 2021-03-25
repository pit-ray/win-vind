cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/proc)
add_library(proc STATIC
    external_app.cpp
    open_window.cpp
)
target_link_libraries(proc psapi)
enable_cotire(proc)

