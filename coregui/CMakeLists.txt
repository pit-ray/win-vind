cmake_minimum_required(VERSION 3.0.0)

string(REPLACE "." "," FILE_VERSION "${PROJECT_VERSION}")
configure_file(
    ${PROJECT_SOURCE_DIR}/res/build_assets/version.hpp.in
    ${PROJECT_SOURCE_DIR}/coregui/include/version.hpp
    NEWLINE_STYLE LF
    @ONLY
)

configure_file(
    ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc.in
    ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc
    NEWLINE_STYLE LF
    @ONLY
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/libs
    ${wxWidgets_INCLUDE_DIRS}
)


set(SOURCE_FILES
    src/about.cpp
    src/font.cpp
    src/main.cpp
    src/startup.cpp
    src/tray.cpp
    src/update.cpp
)

#build all
if(${MSVC})
    set(winresobj ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc)

    add_executable(${PROJECT_NAME} WIN32
        ${winresobj}
        ${SOURCE_FILES}
    )

else()
    if(NOT CMAKE_RC_COMPILER)
       set(CMAKE_RC_COMPILER windres.exe)
    endif()

    #build the file properties
    set(winres "${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc")

    set(winresobj "${CMAKE_CURRENT_BINARY_DIR}/resource.rc.obj")
    add_custom_command(OUTPUT "${winresobj}"
       COMMAND ${CMAKE_RC_COMPILER}
       -D GCC_WINDRES
       -I ${CMAKE_CURRENT_SOURCE_DIR}
       -I ${CMAKE_CURRENT_BINARY_DIR}
       -o ${winresobj}
       -i ${winres}
    )

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        message(STATUS "Enable Console")
        add_executable(${PROJECT_NAME}
            ${winresobj}
            ${SOURCE_FILES}
        )
    else()
        add_executable(${PROJECT_NAME} WIN32
            ${winresobj}
            ${SOURCE_FILES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} vindcore)
link_wxWidgets(${PROJECT_NAME})
enable_cotire(${PROJECT_NAME})
