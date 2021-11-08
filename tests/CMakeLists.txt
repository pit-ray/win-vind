cmake_minimum_required(VERSION 3.0.0)
project(win-vind-test VERSION 0.0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${MSVC})
    add_compile_options(/W4 /std:c++17 /MT /D_UNICODE /DUNICODE /DEBUG /DDEBUG)
    add_link_options(/FORCE:MULTIPLE)
else()
    set(CMAKE_CXX_FLAGS "-static -lstdc++ -lstdc++fs -lgcc -fdiagnostics-color -DUNICODE -D_UNICODE -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 --coverage -m64 -O0 -g3 -DDEBUG -Wl,--allow-multiple-definition")
endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")

set(ROOT_DIR ..)

include_directories(
    ${ROOT_DIR}/src
    ${ROOT_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest
    ${CMAKE_CURRENT_SOURCE_DIR}/fff
)
add_library(doctest STATIC doctest.cpp)

file(GLOB NORMAL_TEST_CODES LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/normal/*.cpp)
add_executable(normal-test ${NORMAL_TEST_CODES})
target_link_libraries(normal-test doctest)
add_test(
    NAME normal-test
    COMMAND $<TARGET_FILE:normal-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(mock-string-test ${CMAKE_CURRENT_SOURCE_DIR}/mock/string_test.cpp)
target_link_libraries(mock-string-test doctest)
add_test(
    NAME mock-string-test
    COMMAND $<TARGET_FILE:mock-string-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(mock-winwrap-test ${CMAKE_CURRENT_SOURCE_DIR}/mock/winwrap_test.cpp)
target_link_libraries(mock-winwrap-test doctest)
add_test(
    NAME mock-winwrap-test
    COMMAND $<TARGET_FILE:mock-winwrap-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(mock-err-logger-test ${CMAKE_CURRENT_SOURCE_DIR}/mock/err_logger_test.cpp)
target_link_libraries(mock-err-logger-test doctest)
add_test(
    NAME mock-err-logger-test
    COMMAND $<TARGET_FILE:mock-err-logger-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS} --verbose --no-label-summary --parallel ${PROC_N}")
