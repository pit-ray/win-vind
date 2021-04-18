cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
)

add_library(coreio STATIC
    src/coreio/err_logger.cpp
    src/coreio/i_params.cpp
    src/coreio/path.cpp
)
target_link_libraries(coreio Userenv)

add_library(util STATIC
    src/util/color.cpp
    src/util/debug.cpp
    src/util/string.cpp
    src/util/winwrap.cpp
)
target_link_libraries(util coreio)

add_library(key STATIC
    src/key/key_absorber.cpp
    src/key/key_log.cpp
    src/key/keycodecvt.cpp
)
target_link_libraries(key util)

add_library(io STATIC
    src/io/display_text_painter.cpp
    src/io/keybrd.cpp
    src/io/mouse.cpp
    src/io/screen_metrics.cpp
)
target_link_libraries(io key)

add_library(time STATIC
    src/time/interval_timer.cpp
    src/time/keystroke_repeater.cpp
)

add_library(base STATIC
    src/bind/base/binded_func.cpp
    src/bind/base/char_logger.cpp
    src/bind/base/key_logger_base.cpp
    src/bind/base/mode.cpp
    src/bind/base/ntype_logger.cpp
)
target_link_libraries(base time io)

add_library(ctrl STATIC
    src/bind/ctrl/dedicate_to_window_ctrl.cpp
    src/bind/ctrl/mywindow_ctrl.cpp
) 
target_link_libraries(ctrl base)

add_library(dev STATIC
    src/bind/dev/mybindings.cpp
)
target_link_libraries(dev base)

add_library(emu STATIC
    src/bind/emu/edi_change_mode.cpp
    src/bind/emu/edi_edit.cpp
    src/bind/emu/edi_jump_caret.cpp
    src/bind/emu/edi_layout.cpp
    src/bind/emu/edi_motion.cpp
    src/bind/emu/edi_move_caret.cpp
    src/bind/emu/edi_replace.cpp
    src/bind/emu/edi_search_pattern.cpp
    src/bind/emu/simple_text_selecter.cpp
    src/bind/emu/smart_clipboard.cpp
    src/bind/emu/text_analyzer.cpp
)
target_link_libraries(emu base)

add_library(file STATIC
    src/bind/file/filer.cpp
    src/bind/file/mkdir.cpp
)
target_link_libraries(file base)

add_library(hotkey STATIC
    src/bind/hotkey/clipboard.cpp
    src/bind/hotkey/pager.cpp
    src/bind/hotkey/search_pattern.cpp
    src/bind/hotkey/switch_vdesktop.cpp
    src/bind/hotkey/tab_ctrl.cpp
    src/bind/hotkey/undo.cpp
)
target_link_libraries(hotkey base)

add_library(mode STATIC
    src/bind/mode/change_mode.cpp
    src/bind/mode/command_mode.cpp
)
target_link_libraries(mode base)

add_library(mouse STATIC
    src/bind/mouse/click.cpp
    src/bind/mouse/jump_actwin.cpp
    src/bind/mouse/jump_cursor.cpp
    src/bind/mouse/jump_keybrd.cpp
    src/bind/mouse/move_cursor.cpp
    src/bind/mouse/scroll.cpp
    src/bind/mouse/select.cpp
)
target_link_libraries(mouse base)

add_library(proc STATIC
    src/bind/proc/external_app.cpp
    src/bind/proc/open_window.cpp
)
target_link_libraries(proc base psapi)

add_library(uia STATIC
    src/bind/uia/easy_click.cpp
    src/bind/uia/uia.cpp
)
target_include_directories(uia PRIVATE ${WINDOWS_SDK_INCLUDE_DIR})
target_link_libraries(uia base)

add_library(window STATIC
    src/bind/window/arrange_window.cpp
    src/bind/window/close_window.cpp
    src/bind/window/exchange_window.cpp
    src/bind/window/min_max_window.cpp
    src/bind/window/reload_window.cpp
    src/bind/window/resize_window.cpp
    src/bind/window/rotate_window.cpp
    src/bind/window/select_window.cpp
    src/bind/window/snap_window.cpp
    src/bind/window/split_open_window.cpp
    src/bind/window/switch_window.cpp
    src/bind/window/window_utility.cpp
)
target_link_libraries(window base Dwmapi)


add_library(bind STATIC
    src/bind/bindings_json_parser.cpp
    src/bind/bindings_lists.cpp
    src/bind/bindings_parser.cpp
    src/bind/func_finder.cpp
    src/bind/global_bindings_caller.cpp
    src/bind/logger_parser.cpp
)
target_link_libraries(bind ctrl emu file hotkey mode mouse proc uia window)
#target_link_libraries(bind dev)

add_library(opt STATIC
    src/opt/autotrack_popup.cpp
    src/opt/dedicate_to_window.cpp
    src/opt/option.cpp
    src/opt/option_loader.cpp
    src/opt/suppress_for_vim.cpp
    src/opt/virtual_cmd_line.cpp
)
target_link_libraries(opt emu mode)

add_library(vindcore STATIC
    src/entry.cpp
)
target_link_libraries(vindcore bind opt)
