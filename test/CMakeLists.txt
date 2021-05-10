cmake_minimum_required(VERSION 3.0.0)
project(win-vind-test VERSION 0.0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${MSVC})
    add_compile_options(/W4 /std:c++latest /MT /D_UNICODE /DUNICODE /DEBUG /DDEBUG)
    add_link_options(/FORCE:MULTIPLE)
else()
    set(CMAKE_CXX_FLAGS "-static -lstdc++ -lgcc -fdiagnostics-color -DUNICODE -D_UNICODE -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 --coverage -m64 -O0 -g3 -DDEBUG -Wl,--allow-multiple-definition")
endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")

set(ROOT_DIR ..)

include_directories(
    ${ROOT_DIR}/core/include
    ${ROOT_DIR}/core/src
    ${ROOT_DIR}
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
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(core-util-string-mock-test
    core/util_string_test.cpp
)
target_link_libraries(core-util-string-mock-test doctest)
add_test(
    NAME core-util-string-mock-test
    COMMAND $<TARGET_FILE:core-util-string-mock-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(core-util-winwrap-mock-test
    core/util_winwrap_test.cpp
)
target_link_libraries(core-util-winwrap-mock-test doctest)
add_test(
    NAME core-util-winwrap-mock-test
    COMMAND $<TARGET_FILE:core-util-winwrap-mock-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

add_executable(core-coreio-err-mock-test
    core/coreio_err_logger_test.cpp
)
target_link_libraries(core-coreio-err-mock-test doctest)
add_test(
    NAME core-coreio-err-mock-test
    COMMAND $<TARGET_FILE:core-coreio-err-mock-test>
    WORKING_DIRECTORY ${ROOT_DIR}
)

set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS} --verbose --no-label-summary --parallel ${PROC_N}")
