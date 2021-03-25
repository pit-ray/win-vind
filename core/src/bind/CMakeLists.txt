cmake_minimum_required(VERSION 3.0.0)

set(BIND_INCLUDE_DIR ${CORE_INCLUDE_DIR}/bind)
include_directories(${BIND_INCLUDE_DIR})

add_subdirectory(ctrl)
add_subdirectory(emu)
add_subdirectory(file)
add_subdirectory(mode)
add_subdirectory(mouse)
add_subdirectory(proc)
add_subdirectory(uia)
add_subdirectory(utility)
add_subdirectory(window)

add_library(bind STATIC
    binded_func.cpp
    key_binder.cpp
    key_matcher.cpp
    mode_manager.cpp
)
target_link_libraries(bind ctrl emu file mode mouse proc uia utility window)
enable_cotire(bind)
