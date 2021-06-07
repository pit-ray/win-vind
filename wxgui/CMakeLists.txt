cmake_minimum_required(VERSION 3.0.0)

set(SUB_PROJECT_NAME ${PROJECT_NAME}-config)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/settings_ctrls
    ${CMAKE_CURRENT_SOURCE_DIR}/include/settings_ctrls/ctrl_assets

    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/libs

    ${wxWidgets_INCLUDE_DIRS}
)

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_FILES
    ${SRCDIR}/io_params.cpp
    ${SRCDIR}/ui_translator.cpp

    ${SRCDIR}/settings_ctrls/ctrl_assets/checkbox_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/choice_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/clrpicker_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/filepicker_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/slider_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/spin_wl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_assets/text_wl.cpp

    ${SRCDIR}/settings_ctrls/choices_manager.cpp
    ${SRCDIR}/settings_ctrls/common_ctrl.cpp
    ${SRCDIR}/settings_ctrls/ctrl_core.cpp
    ${SRCDIR}/settings_ctrls/easy_click_ctrl.cpp
    ${SRCDIR}/settings_ctrls/move_cursor_ctrl.cpp
    ${SRCDIR}/settings_ctrls/options_ctrl.cpp
    ${SRCDIR}/settings_ctrls/scroll_page_ctrl.cpp
    ${SRCDIR}/settings_ctrls/virtual_cmd_line_ctrl.cpp
    ${SRCDIR}/settings_ctrls/resize_window_ctrl.cpp

    ${SRCDIR}/wx_bindings.cpp
    ${SRCDIR}/wx_panel_core.cpp
    ${SRCDIR}/wx_prop_dlg.cpp
    ${SRCDIR}/wx_settings.cpp
    ${SRCDIR}/wx_shortcut_apps.cpp
    ${SRCDIR}/wxgui.cpp

    ${SRCDIR}/main.cpp
)

#build all
include_directories(${MAIN_ENTRY_INCLUDE})
#build all
if(${MSVC})
    set(winresobj ${PROJECT_SOURCE_DIR}/build_resources/resource.rc)

else()
    if(NOT CMAKE_RC_COMPILER)
       set(CMAKE_RC_COMPILER windres.exe)
    endif()

    #build the file properties
    set(winres "${PROJECT_SOURCE_DIR}/build_resources/resource.rc")

    set(winresobj "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.obj")
    add_custom_command(OUTPUT "${winresobj}"
       COMMAND ${CMAKE_RC_COMPILER}
       -D GCC_WINDRES
       -I ${CMAKE_CURRENT_SOURCE_DIR}
       -I ${CMAKE_CURRENT_BINARY_DIR}
       -o ${winresobj}
       -i ${winres}
    )
endif()

add_executable(${SUB_PROJECT_NAME} WIN32
    ${winresobj}
    ${SOURCE_FILES}
)
target_link_libraries(${SUB_PROJECT_NAME} kernel)
link_wxWidgets(${SUB_PROJECT_NAME})
enable_cotire(${SUB_PROJECT_NAME})
