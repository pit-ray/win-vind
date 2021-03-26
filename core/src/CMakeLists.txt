cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CORE_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/libs
)

add_library(vindcore STATIC
    i_params.cpp
    err_logger.cpp
    path.cpp
    entry.cpp
)

add_subdirectory(bind)
add_subdirectory(io)
add_subdirectory(key)
add_subdirectory(opt)
add_subdirectory(text)
add_subdirectory(time)
add_subdirectory(util)

target_link_libraries(vindcore bind Userenv opt io key text time util)
enable_cotire(vindcore)
