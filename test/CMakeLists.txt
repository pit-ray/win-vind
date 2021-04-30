cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/core/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest
)
add_library(doctest STATIC
    main.cpp
)

add_executable(core-test
    core/util-color-test.cpp
    core/util-container-test.cpp
    core/util-math-test.cpp
    core/util-string-test.cpp
    core/util-winwrap-test.cpp
)
target_link_libraries(core-test doctest)
