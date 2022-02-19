cmake_minimum_required(VERSION 3.6.0)


function(get_winsdk_root output)
    get_filename_component(_WINDOWS_SDK_ROOT_DIR
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" REALPATH)
    set(${output} ${_WINDOWS_SDK_ROOT_DIR} PARENT_SCOPE)
endfunction()


function(get_latest_sdk winsdk_root category output)
    file(GLOB winsdk_list LIST_DIRECTORIES true "${winsdk_root}/${category}/10.0.*.*")
    list(SORT winsdk_list ORDER DESCENDING)
    list(GET winsdk_list 0 winsdk_dir)
    set(${output} ${winsdk_dir} PARENT_SCOPE)
endfunction()


function(get_winsdk_include output)
    get_winsdk_root(winsdk_root)
    get_latest_sdk(${winsdk_root} "Include" winsdk_include)

    if(NOT EXISTS ${winsdk_include})
        message(FATAL_ERROR
            "Could not find the directory (${winsdk_include})."
            "You can fix by the cmake flag; -D${output}=<Path>")
    endif()
    message(STATUS "Detected Windows SDK (Include): ${winsdk_include}")

    set(${output} ${winsdk_include} PARENT_SCOPE)
endfunction()


function(get_winsdk_lib output)
    get_winsdk_root(winsdk_root)
    get_latest_sdk(${winsdk_root} "Lib" winsdk_lib)

    if(NOT EXISTS ${winsdk_lib})
        message(FATAL_ERROR
            "Could not find the directory (${winsdk_lib})."
            "You can fix by the cmake flag; -D${output}=<Path>")
    endif()
    message(STATUS "Detected Windows SDK (Lib): ${winsdk_lib}")

    set(${output} ${winsdk_lib} PARENT_SCOPE)
endfunction()


function(icu_absolute_x64 winsdk_libdir output)
    set(${output} ${winsdk_libdir}/um/x64/icu.lib PARENT_SCOPE)
endfunction()


function(enable_ccache)
    find_program(CCACHE ccache)
    if(CCACHE)
        if(${CCACHE_ENABLE})
            if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
                message(STATUS "Enable ccache")
                if(CMAKE_CXX_COMPILER_LAUNCHER)
                    set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER}" "${CCACHE}")
                else()
                    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE}")
                endif()
            endif()
        endif()
    endif()
endfunction()
