cmake_minimum_required(VERSION 3.0.0)
project(win-vind VERSION 0.1.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wdouble-promotion -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wreorder -Wshadow -Wsuggest-override -Wuseless-cast")

include(CTest)
enable_testing()

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

include_directories(
    include
    include/keybinds
    include/editor_keybinds
    include/options
    C:/boost
)

add_executable(${PROJECT_NAME} WIN32
    src/main.cpp
    "${winresobj}"
)

add_subdirectory(src)
target_link_libraries(${PROJECT_NAME} Main)

install(TARGETS win-vind RUNTIME DESTINATION .)
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/log
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    DESTINATION .
)

#NSIS
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR NSIS)

set(CPACK_PACKAGE_NAME "win-vind")
set(CPACK_PACKAGE_VENDOR "pit-ray")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "win-vind")
set(CPACK_PACKAGE_VERSION_MAJOR "1.0.0")
set(CPACK_PACKAGE_VERSION_MINOR "0.0.1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_VERSION "1.0.1-alpha")
set(CPACK_PACKAGE_FILE_NAME "setup_win-vind_1.0.1_alpha")
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