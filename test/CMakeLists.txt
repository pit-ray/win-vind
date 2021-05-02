cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/core/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest
    ${CMAKE_CURRENT_SOURCE_DIR}/fff
)
add_library(doctest STATIC
    main.cpp
)

add_executable(core-test
    core/util_color_test.cpp
    core/util_container_test.cpp
    core/util_debug_test.cpp
    core/util_math_test.cpp
    core/util_string_test.cpp
    core/util_winwrap_test.cpp

    core/time_interval_timer_test.cpp
    core/time_keystroke_repeater_test.cpp

    core/coreio_err_logger_test.cpp
)
target_link_libraries(core-test doctest)
