cmake_minimum_required(VERSION 3.0.0)
project(win-vind VERSION 0.1.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wdouble-promotion -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wreorder -Wshadow -Wsuggest-override -Wuseless-cast -static -lstdc++ -lgcc -lwinpthread")

set(winres "${CMAKE_CURRENT_SOURCE_DIR}/resources/resource.rc")
if(NOT CMAKE_RC_COMPILER)
    set(CMAKE_RC_COMPILER windres.exe)
endif()

set(winresobj "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.obj")
add_custom_command(OUTPUT "${winresobj}"
    COMMAND ${CMAKE_RC_COMPILER}
    -D GCC_WINDRES
    -I ${CMAKE_CURRENT_SOURCE_DIR}
    -I ${CMAKE_CURRENT_BINARY_DIR}
    -o ${winresobj}
    -i ${winres}
)

#For Debug------------------------
#ON is GUI by Win32API.
#OFF is GUI by wxWidgets.
set(USE_SIMPL_GUI OFF)
set(ALLOW_CONSOLE OFF)
#---------------------------------

include_directories(
    libs
    include
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/system)

#main
if(${USE_SIMPL_GUI})
    include_directories(include/win32gui)

    set(W32G_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/win32gui)
    set(WIN32GUI_SRC
        ${W32G_DIR}/main.cpp
        ${W32G_DIR}/win32gui.cpp
    )

    set(EXE_SRC ${WIN32GUI_SRC} "${winresobj}")

    if(${ALLOW_CONSOLE})
        add_executable(${PROJECT_NAME} ${EXE_SRC})
    else()
        add_executable(${PROJECT_NAME} WIN32 ${EXE_SRC})
    endif()

    target_link_libraries(${PROJECT_NAME} CoreSystem)

else()
    set(wxWidgets_USE_DEBUG ON)
    set(wxWidgets_USE_UNICODE ON)
    set(wxWidgets_USE_UNIVERSAL OFF)
    set(wxWidgets_USE_STATIC ON)

    find_package(wxWidgets COMPONENTS adv core base REQUIRED)
    include(${wxWidgets_USE_FILE})

    include_directories(
        include/wxgui
        include/wxgui/settings_ctrls
        C:/wxWidgets/include
    )

    set(WXG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/wxgui)
    set(WXG_SETCTR_DIR ${WXG_DIR}/settings_ctrls)
    set(WXGUI_SRC
        src/i_params.cpp

        ${WXG_DIR}/ui_translator.cpp
        ${WXG_DIR}/io_params.cpp

        ${WXG_SETCTR_DIR}/ctrl_core.cpp
        ${WXG_SETCTR_DIR}/choices_manager.cpp
        ${WXG_SETCTR_DIR}/common_ctrl.cpp
        ${WXG_SETCTR_DIR}/options_ctrl.cpp
        ${WXG_SETCTR_DIR}/move_cursor_ctrl.cpp
        ${WXG_SETCTR_DIR}/scroll_page_ctrl.cpp
        ${WXG_SETCTR_DIR}/virtual_cmd_line_ctrl.cpp

        ${WXG_DIR}/wx_panel_core.cpp
        ${WXG_DIR}/wx_system_tray.cpp
        ${WXG_DIR}/wx_prop_dlg.cpp
        ${WXG_DIR}/wx_settings.cpp
        ${WXG_DIR}/wx_bindings.cpp
        ${WXG_DIR}/wx_shortcut_apps.cpp
        ${WXG_DIR}/wxgui.cpp
        ${WXG_DIR}/main.cpp
    )
    set(EXE_SRC ${WXGUI_SRC} "${winresobj}")

    if(${ALLOW_CONSOLE})
        add_executable(${PROJECT_NAME} ${EXE_SRC})
    else()
        add_executable(${PROJECT_NAME} WIN32 ${EXE_SRC})
    endif()

    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} CoreSystem)
endif()


#CPack
#-----------------------------------------------------------------------------
install(TARGETS win-vind RUNTIME DESTINATION .)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/log
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/default_config
    DESTINATION .
)

#NSIS
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR NSIS)

set(CPACK_PACKAGE_NAME "win-vind")
set(CPACK_PACKAGE_VENDOR "pit-ray")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win-vind")
set(CPACK_PACKAGE_VERSION_MAJOR "2.0.0")
set(CPACK_PACKAGE_VERSION_MINOR "0.0.0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_VERSION "2.0.0")
set(CPACK_PACKAGE_FILE_NAME "setup_win-vind_2.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "win-vind")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "applications (win-vind)")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/installer512.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/resources/uninstaller512.ico")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)


list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMSTARTUP\\\\win-vind.lnk' '$INSTDIR\\\\win-vind.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateShortCut '$SMPROGRAMS\\\\win-vind\\\\win-vind.lnk' '$INSTDIR\\\\win-vind.exe'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CreateDirectory '$PROFILE\\\\.win-vind'")
list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "CopyFiles '$INSTDIR\\\\config\\\\*' '$PROFILE\\\\.win-vind'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")

list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMSTARTUP\\\\win-vind.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "Delete '$SMPROGRAMS\\\\win-vind\\\\win-vind.lnk'")
list(APPEND CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "RMDir /r '$PROFILE\\\\.win-vind'")
string(REPLACE ";" "\n" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS}")
include(CPack)