cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/mouse)
add_library(mouse STATIC
    click.cpp
    jump_actwin.cpp
    jump_cursor.cpp
    jump_keybrd.cpp
    move_cursor.cpp
    scroll.cpp
    select.cpp
)
enable_cotire(mouse)


