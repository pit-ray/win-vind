cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/opt)
add_library(opt STATIC
    autotrack_popup.cpp
    dedicate_to_window.cpp
    option.cpp
    option_loader.cpp
    suppress_for_vim.cpp
    virtual_cmd_line.cpp
)
enable_cotire(opt)

