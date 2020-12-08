cmake_minimum_required(VERSION 3.0.0)

set(MAIN_ENTRY_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/libs

    PARENT_SCOPE
)

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MAIN_ENTRY
    ${SRCDIR}/win32gui.cpp
    ${SRCDIR}/main.cpp

    ${winresobj}

    PARENT_SCOPE
)
