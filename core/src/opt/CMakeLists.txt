cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/opt)
add_library(opt STATIC
    analyze_text.cpp
    autotrack_popup.cpp
    dedicate_to_window.cpp
    dynamic_option.cpp
    option_loader.cpp
    suppress_for_vim.cpp
    virtual_cmd_line.cpp
)
enable_cotire(opt)

