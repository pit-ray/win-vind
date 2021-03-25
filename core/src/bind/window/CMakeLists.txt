cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/window)
add_library(window STATIC
    arrange_window.cpp
    close_window.cpp
    exchange_window.cpp
    min_max_window.cpp
    reload_window.cpp
    resize_window.cpp
    rotate_window.cpp
    select_window.cpp
    snap_window.cpp
    split_open_window.cpp
    switch_window.cpp
    window_utility.cpp
)
target_link_libraries(window Dwmapi)
enable_cotire(window)
