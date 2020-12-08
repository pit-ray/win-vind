cmake_minimum_required(VERSION 3.0.0)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(wxWidgets_USE_DEBUG ON)
else()
    set(wxWidgets_USE_DEBUG OFF)
endif()
set(wxWidgets_USE_UNICODE ON)
set(wxWidgets_USE_UNIVERSAL OFF)
set(wxWidgets_USE_STATIC ON)

find_package(wxWidgets COMPONENTS adv core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/settings_ctrls

    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/libs
)

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(wxGUI STATIC
    ${SRCDIR}/io_params.cpp
    ${SRCDIR}/ui_translator.cpp

    ${SRCDIR}/settings_ctrls/choices_manager.cpp
    ${SRCDIR}/settings_ctrls/common_ctrl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_core.cpp
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
target_link_libraries(wxGUI ${wxWidgets_LIBRARIES})

set(MAIN_ENTRY_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/include

    ${wxWidgets_INCLUDE_DIRS}
    PARENT_SCOPE
)
set(MAIN_ENTRY ${SRCDIR}/main.cpp PARENT_SCOPE)
set(MAIN_ENTRY_LIBRARIES wxGUI PARENT_SCOPE)
