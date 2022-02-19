cmake_minimum_required(VERSION 3.6.0)

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
find_package(wxWidgets COMPONENTS html adv core base REQUIRED)

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
    ${WINDOWS_SDK_INCLUDE_DIR}
    ${wxWidgets_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false *.cpp)

if(${MSVC})
    set(winresobj ${PROJECT_SOURCE_DIR}/res/build_assets/resource.rc)

    add_executable(${PROJECT_NAME} WIN32
        ${winresobj}
        ${SOURCE_FILES}
    )

    set(ICU_LIB icu)

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

    set(ICU_LIB ${WINDOWS_SDK_LIB_DIR}/um/x64/icu.lib)
endif()


link_wxWidgets(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
    psapi
    dwmapi
    userenv
    ${ICU_LIB}
)
