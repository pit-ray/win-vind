cmake_minimum_required(VERSION 3.0.0)
include_directories(${BIND_INCLUDE_DIR}/emu)
add_library(emu STATIC
    edi_change_mode.cpp
    edi_edit.cpp
    edi_jump_caret.cpp
    edi_layout.cpp
    edi_move_caret.cpp
    edi_replace.cpp
    edi_search_pattern.cpp
)
enable_cotire(emu)
