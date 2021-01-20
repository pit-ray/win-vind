cmake_minimum_required(VERSION 3.0.0)
project(win-vind VERSION 3.0.2)

if(NOT BIT_TYPE)
    set(BIT_TYPE 64)
endif()

option(COTIRE_ENABLE OFF)
option(CCACHE_ENABLE ON)
option(USE_SIMPL_GUI OFF)

enable_language(CXX)

string(REPLACE "." "," FILE_VERSION "${PROJECT_VERSION}")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/build_resources/version.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/wxgui/include/version.hpp
    NEWLINE_STYLE LF
    @ONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/build_resources/resource.rc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/build_resources/resource.rc
    NEWLINE_STYLE LF
    @ONLY
)

if(${MSVC})
    add_compile_options(/W4 /std:c++latest /MT /D_UNICODE /DUNICODE)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        add_compile_options(/DEBUG)
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_compile_options(/O2)
    endif()

    set(winresobj ${CMAKE_CURRENT_SOURCE_DIR}/build_resources/resource.rc)

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
    )
    list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS)
    string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    if(${BIT_TYPE} STREQUAL 32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    endif()

    message(STATUS "g++ common flag: ${CMAKE_CXX_FLAGS}")

    set(CMAKE_CXX_DEBUG "-O0 -g3")
    set(CMAKE_CXX_RELEASE "-O2")

    if(NOT CMAKE_RC_COMPILER)
       set(CMAKE_RC_COMPILER windres.exe)
    endif()

    #build the file properties
    set(winres "${CMAKE_CURRENT_SOURCE_DIR}/build_resources/resource.rc")

    set(winresobj "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.obj")
    add_custom_command(OUTPUT "${winresobj}"
       COMMAND ${CMAKE_RC_COMPILER}
       -D GCC_WINDRES
       -I ${CMAKE_CURRENT_SOURCE_DIR}
       -I ${CMAKE_CURRENT_BINARY_DIR}
       -o ${winresobj}
       -i ${winres}
    )

    set(CMAKE_SH "CMAKE_SH-NOTFOUND")
endif()

include(ProcessorCount)
ProcessorCount(PROC_N)
message(STATUS "The number of processors: " ${PROC_N})
set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${PROC_N}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

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

#add core system
add_subdirectory(core)

#add entry with GUI
set(ENTRY_CMAKE_SOURCE wxgui)
if(${USE_SIMPL_GUI})
    message(STATUS "Use Simple GUI System")
    set(ENTRY_CMAKE_SOURCE win32gui)
endif()
add_subdirectory(${ENTRY_CMAKE_SOURCE})

#build all
include_directories(${MAIN_ENTRY_INCLUDE})
if(${MSVC})
    add_executable(${PROJECT_NAME} WIN32 ${MAIN_ENTRY} ${winresobj})

else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(STATUS "Enable Console")
        add_executable(${PROJECT_NAME} ${MAIN_ENTRY} ${winresobj})
    else()
        add_executable(${PROJECT_NAME} WIN32 ${MAIN_ENTRY} ${winresobj})
    endif()
endif()

target_link_libraries(${PROJECT_NAME} WinVindGUI WinVindCore)
enable_cotire(${PROJECT_NAME})

#CPack-----------------------------------------------------------------------------
install(TARGETS win-vind RUNTIME DESTINATION .)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/log
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/default_config
    DESTINATION .
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CPACK_GENERATOR NSIS)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "pit-ray")
set(CPACK_PACKAGE_COPYRIGHT "Copyright (c) 2020 pit-ray")
set(CPACK_PACKAGE_DESCRIPTION "win-vind - Simple Vim Key Binder for Windows (${BIT_TYPE}-bit)")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://pit-ray.github.io/win-vind/")
set(CPACK_PACKAGE_FILE_NAME "setup_${PROJECT_NAME}_${PROJECT_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win-vind")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/build_resources/installer512.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/build_resources/uninstaller512.ico")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "applications (${PROJECT_NAME})")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)
set(CPACK_NSIS_PACKAGE_NAME ${PRODUCT_NAME})
set(CPACK_NSIS_PACKAGE_VERSION ${PROJECT_VERSION}.0)

#NSIS
set(CPACK_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build_resources) #Use custom installer template

list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMSTARTUP\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\${CPACK_PACKAGE_NAME}.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\${CPACK_PACKAGE_NAME}.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateDirectory '$PROFILE\\\\.${CPACK_PACKAGE_NAME}'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CopyFiles '$INSTDIR\\\\config\\\\*' '$PROFILE\\\\.${CPACK_PACKAGE_NAME}'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")

list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMSTARTUP\\\\${CPACK_PACKAGE_NAME}.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMPROGRAMS\\\\${CPACK_PACKAGE_NAME}\\\\${CPACK_PACKAGE_NAME}.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$PROFILE\\\\.${CPACK_PACKAGE_NAME}'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")

include(CPack)
