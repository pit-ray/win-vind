cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/text)
add_library(text STATIC
    simple_text_selecter.cpp
    smart_clipboard.cpp
    text_analyzer.cpp
)
enable_cotire(text)

