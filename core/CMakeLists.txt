cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
)

add_library(util STATIC
    src/util/color.cpp
    src/util/debug.cpp
    src/util/string.cpp
    src/util/winwrap.cpp
)

add_library(key STATIC
    src/key/char_logger.cpp
    src/key/key_absorber.cpp
    src/key/key_log.cpp
    src/key/key_logger_base.cpp
    src/key/keycodecvt.cpp
    src/key/log_map.cpp
    src/key/ntype_logger.cpp
)

add_library(io STATIC
    src/io/display_text_painter.cpp
    src/io/keybrd.cpp
    src/io/mouse.cpp
    src/io/screen_metrics.cpp
)

add_library(time STATIC
    src/time/constant_accelerator.cpp
    src/time/interval_timer.cpp
    src/time/keystroke_repeater.cpp
)

add_library(ctrl STATIC
    src/bind/ctrl/dedicate_to_window_ctrl.cpp
    src/bind/ctrl/mywindow_ctrl.cpp
) 

add_library(dev STATIC
    src/bind/dev/mybindings.cpp
)

add_library(emu STATIC
    src/bind/emu/edi_change.cpp
    src/bind/emu/edi_change_mode.cpp
    src/bind/emu/edi_delete.cpp
    src/bind/emu/edi_jump_caret.cpp
    src/bind/emu/edi_layout.cpp
    src/bind/emu/edi_motion.cpp
    src/bind/emu/edi_move_caret.cpp
    src/bind/emu/edi_put.cpp
    src/bind/emu/edi_replace.cpp
    src/bind/emu/edi_yank.cpp
    src/bind/emu/simple_text_register.cpp
    src/bind/emu/simple_text_selecter.cpp
    src/bind/emu/smart_clipboard.cpp
    src/bind/emu/text_analyzer.cpp
    src/bind/emu/text_util.cpp
)

add_library(file STATIC
    src/bind/file/explorer_util.cpp
    src/bind/file/filer.cpp
    src/bind/file/mkdir.cpp
)

add_library(hotkey STATIC
    src/bind/hotkey/clipboard.cpp
    src/bind/hotkey/pager.cpp
    src/bind/hotkey/search_pattern.cpp
    src/bind/hotkey/switch_vdesktop.cpp
    src/bind/hotkey/tab_ctrl.cpp
    src/bind/hotkey/undo.cpp
)

add_library(mode STATIC
    src/bind/mode/change_mode.cpp
    src/bind/mode/command_mode.cpp
)

add_library(mouse STATIC
    src/bind/mouse/click.cpp
    src/bind/mouse/jump_actwin.cpp
    src/bind/mouse/jump_cursor.cpp
    src/bind/mouse/jump_keybrd.cpp
    src/bind/mouse/move_cursor.cpp
    src/bind/mouse/scroll.cpp
    src/bind/mouse/select.cpp
)

add_library(proc STATIC
    src/bind/proc/external_app.cpp
    src/bind/proc/open_window.cpp
)
target_link_libraries(proc psapi)

add_library(uia STATIC
    src/bind/uia/display_hinter.cpp
    src/bind/uia/easy_click.cpp
    src/bind/uia/easy_click_core.cpp
    src/bind/uia/ec_hints.cpp
    src/bind/uia/input_hinter.cpp
    src/bind/uia/ui_scanner.cpp
    src/bind/uia/uia.cpp
)
target_include_directories(uia PRIVATE ${WINDOWS_SDK_INCLUDE_DIR})

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
    src/bind/window/window_resizer.cpp
    src/bind/window/window_utility.cpp
)
target_link_libraries(window Dwmapi)

add_library(syscmd STATIC
    src/bind/syscmd/command.cpp
    src/bind/syscmd/map.cpp
    src/bind/syscmd/set.cpp
    src/bind/syscmd/source.cpp
)

add_library(bind STATIC
    src/bind/binded_func.cpp
    src/bind/bindings_lists.cpp
    src/bind/func_finder.cpp
    src/bind/logger_parser.cpp
)
target_link_libraries(bind ctrl emu file hotkey mode mouse proc uia window syscmd)
#target_link_libraries(bind dev)

add_library(opt STATIC
    src/opt/autotrack_popup.cpp
    src/opt/dedicate_to_window.cpp
    src/opt/option.cpp
    src/opt/option_loader.cpp
    src/opt/suppress_for_vim.cpp
    src/opt/vcmdline.cpp
)
target_link_libraries(opt emu mode)

add_library(parser STATIC
    src/parser/bindings_parser.cpp
    src/parser/rc_parser.cpp
)

add_library(kernel STATIC
    src/err_logger.cpp
    src/g_maps.cpp
    src/g_params.cpp
    src/keybrd_layout.cpp
    src/mode.cpp
    src/path.cpp
)
target_link_libraries(kernel parser util Userenv io key time)

add_library(vindcore STATIC
    src/entry.cpp
    src/main_loop.cpp
)
target_link_libraries(vindcore bind opt parser kernel)
