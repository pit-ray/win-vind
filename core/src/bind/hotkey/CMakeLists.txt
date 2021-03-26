cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/hotkey)
add_library(hotkey STATIC
    clipboard.cpp
    pager.cpp
    search_pattern.cpp
    switch_vdesktop.cpp
    tab_ctrl.cpp
    undo.cpp
)
enable_cotire(hotkey)
