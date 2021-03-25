cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/ctrl)
add_library(ctrl STATIC
    dedicate_to_window_ctrl.cpp
    mywindow_ctrl.cpp
) 
enable_cotire(ctrl)
