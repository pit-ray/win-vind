cmake_minimum_required(VERSION 3.6.0)

configure_file(
    ${PROJECT_SOURCE_DIR}/res/build_assets/version.hpp.in
    ${PROJECT_SOURCE_DIR}/src/core/version.hpp
    NEWLINE_STYLE LF
    @ONLY
)

string(REPLACE "." "," FILE_VERSION "${PROJECT_VERSION}")
configure_file(
    ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc.in
    ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc
    NEWLINE_STYLE LF
    @ONLY
)

configure_file(
    ${PROJECT_SOURCE_DIR}/res/build_assets/win-vind.nuspec.in
    ${PROJECT_SOURCE_DIR}/choco/win-vind.nuspec
    NEWLINE_STYLE LF
    @ONLY
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/libs
)

file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false *.cpp)

if(${MSVC})
    set(winresobj ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc)

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
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

else()
    # If you have installed it in Program Files,
    # it will not be escaped properly, so use the environment path.
    set(CMAKE_RC_COMPILER windres)

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

target_link_libraries(${PROJECT_NAME} psapi dwmapi userenv icuuc)
