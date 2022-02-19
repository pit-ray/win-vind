cmake_minimum_required(VERSION 3.0.0)
project(win-vind-test VERSION 0.0.1)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT_DIR ..)

include(${ROOT_DIR}/tools/util.cmake)

if(${MSVC})
    add_compile_options(/W4 /std:c++17 /MT /D_UNICODE /DUNICODE /DEBUG /DDEBUG /utf-8)
    add_link_options(/FORCE:MULTIPLE)

    set(ICU_LIB icu)
else()
    set(CMAKE_CXX_FLAGS "-static -lstdc++ -lstdc++fs -lgcc -fdiagnostics-color -DUNICODE -D_UNICODE -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 --coverage -m64 -O0 -g3 -DDEBUG -Wl,--allow-multiple-definition")

    if(NOT WINDOWS_SDK_INCLUDE_DIR)
        get_winsdk_include(WINDOWS_SDK_INCLUDE_DIR)
    endif()

    if(NOT WINDOWS_SDK_LIB_DIR)
        get_winsdk_lib(WINDOWS_SDK_LIB_DIR)
    endif()

    include_directories(${WINDOWS_SDK_INCLUDE_DIR})
    icu_absolute_x64(${WINDOWS_SDK_LIB_DIR} ICU_LIB)
endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")

include_directories(
    ${ROOT_DIR}/src
    ${ROOT_DIR}/libs
    ${CMAKE_CURRENT_SOURCE_DIR}/doctest/doctest
    ${CMAKE_CURRENT_SOURCE_DIR}/fff
)
add_library(doctest STATIC doctest.cpp)

function(AddTest TEST_NAME)
        add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} doctest)
    add_test(
        NAME ${TEST_NAME}
        COMMAND $<TARGET_FILE:${TEST_NAME}>
        WORKING_DIRECTORY ${ROOT_DIR}
    )
endfunction()

add_subdirectory(normal)
add_subdirectory(mock)
add_subdirectory(each)

set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS} --verbose --no-label-summary --parallel ${PROC_N}")
