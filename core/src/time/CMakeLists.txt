cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/time)
add_library(time STATIC
    interval_timer.cpp
    keystroke_repeater.cpp
)
enable_cotire(time)
