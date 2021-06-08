cmake_minimum_required(VERSION 3.0.0)
project(win-vind VERSION 4.0.0)

if(NOT BIT_TYPE)
    set(BIT_TYPE 64)
endif()

option(COTIRE_ENABLE OFF)
option(CCACHE_ENABLE ON)

enable_language(CXX)

if(${MSVC})
    add_compile_options(/W4 /std:c++latest /MT /D_UNICODE /DUNICODE)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(/DEBUG)
        add_compile_options(/DDEBUG)
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_compile_options(/O2)
    endif()

else()
    if(NOT WINDOWS_SDK_INCLUDE_DIR)
        get_filename_component(WINDOWS_SDK_ROOT_DIR
            "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" REALPATH)
        file(GLOB WINDOWS_SDK_INCS LIST_DIRECTORIES true "${WINDOWS_SDK_ROOT_DIR}/Include/10.0.*.*")
        list(SORT WINDOWS_SDK_INCS ORDER DESCENDING)
        list(GET WINDOWS_SDK_INCS 0 WINDOWS_SDK_INCLUDE_DIR)
    endif()
    if(NOT EXISTS ${WINDOWS_SDK_INCLUDE_DIR})
        message(FATAL_ERROR "Could not find the directory; ${WINDOWS_SDK_INCLUDE_DIR}."
            "You can fix by the cmake flag; -DWINDOWS_SDK_INCLUDE_DIR=<Path>")
    endif()
    message(STATUS "Detected Windows SDK: ${WINDOWS_SDK_INCLUDE_DIR}")

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
        --coverage
    )
    list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    if(${BIT_TYPE} STREQUAL 32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    endif()

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules)

#If use cotire, for some reason, cannot redefine windows version.
#Now, All CMakeLists.txt in win-vind has no header files at add_executable() or add_library()
if(${COTIRE_ENABLE})
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            message(STATUS "Enable cotire")
            set(COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES 1)
            include(cotire)
    endif()
endif()
function(enable_cotire name)
    if(${COTIRE_ENABLE})
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            message(STATUS "${name} is applied cotire")
            cotire(${name})
        endif()
    endif()
endfunction()

find_program(CCACHE ccache)
if(CCACHE)
    if(${CCACHE_ENABLE})
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            message(STATUS "Enable ccache")
            if(CMAKE_CXX_COMPILER_LAUNCHER)
                set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER}" "${CCACHE}")
            else()
                set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
            endif()
        endif()
    endif()
endif()

#add entry with GUI

#Default wxWidgets installed directories
#If you installed to other directories, you can change like the below.
#cmake -DCMAKE_BUILD_TYPE=Debug -DwxWidgets_ROOT_DIR=C:/wxWidgets 
if(NOT wxWidgets_ROOT_DIR)
    set(wxWidgets_ROOT_DIR ${PROJECT_SOURCE_DIR}/libs/wxWidgets)
endif()
if(NOT EXISTS ${wxWidgets_ROOT_DIR})
    message(FATAL_ERROR "Could not find the directory; ${wxWidgets_ROOT_DIR}."
        "You can fix by the cmake flag; -DwxWidgets_ROOT_DIR=<Path>")
endif()
message(STATUS "Detected wxWidgets Root Directory: ${wxWidgets_ROOT_DIR}")

if(NOT wxWidgets_LIB_DIR)
    if(${MSVC})
        if(${BIT_TYPE} STREQUAL 32)
            set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_lib)
        else()
            set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_x64_lib)
        endif()
    else()
        #if(${BIT_TYPE} STREQUAL 32)
            set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/gcc_lib)
        #else()
            #set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/gcc_x64_lib)
        #endif()
    endif()
endif()
if(NOT EXISTS ${wxWidgets_LIB_DIR})
    message(FATAL_ERROR "Could not find the directory; ${wxWidgets_LIB_DIR}."
        "You can fix by the cmake flag; -DwxWidgets_LIB_DIR=<Path>")
endif()
message(STATUS "Detected wxWidgets Library Directory: ${wxWidgets_LIB_DIR}")

if(NOT wxWidgets_CONFIGURATION)
    set(wxWidgets_CONFIGURATION mswu)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(wxWidgets_USE_DEBUG ON)
else()
    set(wxWidgets_USE_DEBUG OFF)
endif()
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_USE_UNIVERSAL OFF)
set(wxWidgets_USE_STATIC ON)

set(wxWidgets_INCLUDE_DIRS ${wxWidgets_ROOT_DIR}/include)
set(_WX_TOOL gcc_x64 FORCE)
find_package(wxWidgets COMPONENTS adv core base REQUIRED)

if(NOT wxWidgets_FOUND)
    message(FATAL_ERROR "Could not find wxWidgets")
endif()

#include(${wxWidgets_USE_FILE})
set(wxWidgets_INCLUDE_DIRS
    ${wxWidgets_INCLUDE_DIRS}
    ${wxWidgets_LIB_DIR}/${wxWidgets_CONFIGURATION}
)

function(link_wxWidgets target)
    if(${wxWidgets_VERSION_MINOR} STREQUAL 1)
        target_link_libraries(${target} ${wxWidgets_LIBRARIES} uxtheme oleacc)
    else()
        target_link_libraries(${target} ${wxWidgets_LIBRARIES})
    endif()
endfunction()

add_subdirectory(core)

add_subdirectory(coregui)

# add_subdirectory(wxgui)

#CPack-----------------------------------------------------------------------------
install(TARGETS win-vind RUNTIME DESTINATION .)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/default_config
    DESTINATION .
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_GENERATOR NSIS)

set(CPACK_PACKAGE_NAME ${CPACK_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${CPACK_PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "pit-ray")
set(CPACK_PACKAGE_COPYRIGHT "Copyright (c) 2020 pit-ray")
set(CPACK_PACKAGE_DESCRIPTION "win-vind - Simple Vim Key Binder for Windows (${BIT_TYPE}-bit)")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://pit-ray.github.io/win-vind/")
set(CPACK_PACKAGE_FILE_NAME "setup_${CPACK_PROJECT_NAME}_${CPACK_PROJECT_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win-vind")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/build_resources/installer512.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/build_resources/uninstaller512.ico")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PROJECT_NAME})
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "applications (${CPACK_PROJECT_NAME})")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL FALSE)
set(CPACK_NSIS_PACKAGE_NAME ${PRODUCT_NAME})
set(CPACK_NSIS_PACKAGE_VERSION ${CPACK_PROJECT_VERSION}.0)

#NSIS
#set(CPACK_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_resources) #Use custom installer template

list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMSTARTUP\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\${CPACK_PACKAGE_NAME}.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\${CPACK_PACKAGE_NAME}.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "Delete '$PROFILE\\\\.${CPACK_PACKAGE_NAME}\\\\is_initialized'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")

list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMSTARTUP\\\\${CPACK_PACKAGE_NAME}.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$PROFILE\\\\.${CPACK_PACKAGE_NAME}'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")

include(CPack)
