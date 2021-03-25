cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/io)
add_library(io STATIC
    display_text_painter.cpp
    keybrd.cpp
    mouse.cpp
    screen_metrics.cpp
)
enable_cotire(io)

