cmake_minimum_required(VERSION 3.0.0)

# -------------
if(${MSVC})
    add_compile_options(/W4 /std:c++latest /MT /D_UNICODE /DUNICODE)
    add_compile_options(/DEBUG)
    add_compile_options(/DDEBUG)
    set(winresobj ${CMAKE_CURRENT_SOURCE_DIR}/build_resources/resource.rc)

else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(CMAKE_CXX_FLAGS 
        -static
        -lstdc++
        -lgcc
        -Wall
        -Wextra
        -Wcast-align
        -Wno-unknown-pragmas
        -Wcast-qual
        -Wctor-dtor-privacy
        -Wdelete-non-virtual-dtor
        -Wdouble-promotion
        -Weffc++
        -Wold-style-cast
        -Woverloaded-virtual
        -Wreorder
        -Wshadow
        -Wsuggest-override
        -Wuseless-cast
        -fdiagnostics-color
        -DUNICODE
        -D_UNICODE
        -Wl,--allow-multiple-definition #UUID of wxWidgets and one of MinGW
        -DWINVER=0x0A00
        -D_WIN32_WINNT=0x0A00
        -Wa,-mbig-obj
    )
    list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -DDEBUG")
    message(STATUS "g++ flags: ${CMAKE_CXX_FLAGS}")
    set(CMAKE_SH "CMAKE_SH-NOTFOUND")

endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")
# -------------

include_directories(
    ../core/include
    ../core/src
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/doctest/doctest
)
add_library(doctest STATIC
    main.cpp
)

add_executable(core-test
    core/util-color-test.cpp
)
target_link_libraries(core-test doctest)
