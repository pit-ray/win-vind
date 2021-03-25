cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CORE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/libs
)

add_library(WinVindCore STATIC
    i_params.cpp
    msg_logger.cpp
    path.cpp
    utility.cpp
    win_vind.cpp
)

add_subdirectory(bind)
add_subdirectory(io)
add_subdirectory(key)
add_subdirectory(opt)
add_subdirectory(text)
add_subdirectory(time)

target_link_libraries(WinVindCore Userenv bind io key opt text time)
enable_cotire(WinVindCore)
