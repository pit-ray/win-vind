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

    core/time_interval_timer_test.cpp
    core/time_keystroke_repeater_test.cpp
)
target_link_libraries(core-test doctest)
add_test(
    NAME core-test
    COMMAND $<TARGET_FILE:core-test>
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(core-util-string-mock-test
    core/util_string_test.cpp
)
target_link_libraries(core-util-string-mock-test doctest)
add_test(
    NAME core-util-string-mock-test
    COMMAND $<TARGET_FILE:core-util-string-mock-test>
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(core-util-winwrap-mock-test
    core/util_winwrap_test.cpp
)
target_link_libraries(core-util-winwrap-mock-test doctest)
add_test(
    NAME core-util-winwrap-mock-test
    COMMAND $<TARGET_FILE:core-util-winwrap-mock-test>
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(core-coreio-err-mock-test
    core/coreio_err_logger_test.cpp
)
target_link_libraries(core-coreio-err-mock-test doctest)
add_test(
    NAME core-coreio-err-mock-test
    COMMAND $<TARGET_FILE:core-coreio-err-mock-test>
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
