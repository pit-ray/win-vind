cmake_minimum_required(VERSION 3.0.0)
include_directories(${CORE_INCLUDE_DIR}/key)
add_library(key STATIC
    char_logger.cpp
    key_absorber.cpp
    key_log.cpp
    key_logger_base.cpp
    vkc_converter.cpp
    vkc_logger.cpp
)
enable_cotire(key)

