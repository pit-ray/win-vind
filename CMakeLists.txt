cmake_minimum_required(VERSION 3.6.0)
project(win-vind VERSION 4.4.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(CCACHE_ENABLE ON)

enable_language(CXX)

include(tools/util.cmake)

get_gen_arch(GEN_ARCH)

if(${MSVC})
    add_compile_options(/W4 /std:c++17 /MT /D_UNICODE /DUNICODE /utf-8)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(/DEBUG)
        add_compile_options(/DDEBUG)
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_compile_options(/O2)
    endif()

else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(CMAKE_CXX_FLAGS 
        -static
        -std=c++17
        -lstdc++
        -lstdc++fs
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
        --coverage
    )
    list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    #if(${CMAKE_GENERATOR_PLATFORM} STREQUAL x64)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    #else()
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    #endif()

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -DDEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()

    message(STATUS "g++ flags: ${CMAKE_CXX_FLAGS}")

    set(CMAKE_SH "CMAKE_SH-NOTFOUND")
endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(NOT WINDOWS_SDK_INCLUDE_DIR)
    get_winsdk_include(WINDOWS_SDK_INCLUDE_DIR)
endif()
include_directories(${WINDOWS_SDK_INCLUDE_DIR})

if(NOT WINDOWS_SDK_LIB_DIR)
    get_winsdk_lib(WINDOWS_SDK_LIB_DIR)
endif()
link_directories(${WINDOWS_SDK_LIB_DIR})

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules)

# enable_ccache()

add_subdirectory(src)

#CPack-----------------------------------------------------------------------------
install(TARGETS win-vind RUNTIME DESTINATION .)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/res/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/res/default_config
    DESTINATION .
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_GENERATOR NSIS)

set(CPACK_PACKAGE_NAME ${CPACK_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CPACK_PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "pit-ray")
set(CPACK_PACKAGE_COPYRIGHT "Copyright (c) 2020-2022 pit-ray")
set(CPACK_PACKAGE_DESCRIPTION "win-vind - Simple Vim Key Binder for Windows (${GEN_ARCH})")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://pit-ray.github.io/win-vind/")
set(CPACK_PACKAGE_FILE_NAME "setup_${CPACK_PROJECT_NAME}_${CPACK_PROJECT_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win-vind")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/installer/installer512.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/res/installer/uninstaller512.ico")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PROJECT_NAME})
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "applications (${CPACK_PROJECT_NAME})")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)
set(CPACK_NSIS_PACKAGE_NAME ${PRODUCT_NAME})
set(CPACK_NSIS_PACKAGE_VERSION ${CPACK_PROJECT_VERSION}.0)


# Register to startmenu
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\${CPACK_PACKAGE_NAME}.exe'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")

include(CPack)
