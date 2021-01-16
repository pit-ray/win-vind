cmake_minimum_required(VERSION 3.0.0)

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

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/settings_ctrls

    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/libs

    ${wxWidgets_INCLUDE_DIRS}
)

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(WinVindGUI STATIC
    ${SRCDIR}/io_params.cpp
    ${SRCDIR}/ui_translator.cpp

    ${SRCDIR}/settings_ctrls/choices_manager.cpp
    ${SRCDIR}/settings_ctrls/common_ctrl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_core.cpp
    ${SRCDIR}/settings_ctrls/easy_click_ctrl.cpp
    ${SRCDIR}/settings_ctrls/move_cursor_ctrl.cpp
    ${SRCDIR}/settings_ctrls/options_ctrl.cpp
    ${SRCDIR}/settings_ctrls/scroll_page_ctrl.cpp
    ${SRCDIR}/settings_ctrls/virtual_cmd_line_ctrl.cpp

    ${SRCDIR}/wx_bindings.cpp
    ${SRCDIR}/wx_panel_core.cpp
    ${SRCDIR}/wx_prop_dlg.cpp
    ${SRCDIR}/wx_settings.cpp
    ${SRCDIR}/wx_shortcut_apps.cpp
    ${SRCDIR}/wx_system_tray.cpp
    ${SRCDIR}/wxgui.cpp
)
if(${wxWidgets_VERSION_MINOR} STREQUAL 1)
    target_link_libraries(WinVindGUI ${wxWidgets_LIBRARIES} uxtheme oleacc)
else()
    target_link_libraries(WinVindGUI ${wxWidgets_LIBRARIES})
endif()

set(MAIN_ENTRY_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/core/include

    ${wxWidgets_INCLUDE_DIRS}
    PARENT_SCOPE
)
set(MAIN_ENTRY ${SRCDIR}/main.cpp PARENT_SCOPE)
