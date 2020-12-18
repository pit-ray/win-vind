cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common
    ${CMAKE_CURRENT_SOURCE_DIR}/include/common_bindings
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gui_bindings
    ${CMAKE_CURRENT_SOURCE_DIR}/include/editor_bindings
    ${CMAKE_CURRENT_SOURCE_DIR}/include/options

    ${PROJECT_SOURCE_DIR}/libs

    ${WINDOWS_SDK_INCLUDE_DIR}
)

add_library(Common STATIC
    src/common/binded_func.cpp
    src/common/dynamic_option.cpp
    src/common/interval_timer.cpp
    src/common/key_absorber.cpp
    src/common/key_log.cpp
    src/common/key_logger.cpp
    src/common/key_matcher.cpp
    src/common/keybrd_eventer.cpp
    src/common/keystroke_repeater.cpp
    src/common/mode_manager.cpp
    src/common/mouse_eventer.cpp
    src/common/simpl_text_selecter.cpp
    src/common/smart_clipboard.cpp
    src/common/text_analyzer.cpp
    src/common/uia.cpp
    src/common/vkc_converter.cpp
)
enable_cotire(Common)

add_library(CmnBindings STATIC
    src/common_bindings/change_mode.cpp
    src/common_bindings/command_mode.cpp
    src/common_bindings/dedicate_to_window_ctrl.cpp
    src/common_bindings/external_app.cpp
    src/common_bindings/filer.cpp
    src/common_bindings/mywindow_ctrl.cpp
    src/common_bindings/scroll.cpp
    src/common_bindings/undo.cpp
    src/common_bindings/window_ctrl.cpp
)
target_link_libraries(CmnBindings Common)
enable_cotire(CmnBindings)

add_library(EdiBindings STATIC
    src/editor_bindings/edi_change_mode.cpp
    src/editor_bindings/edi_edit.cpp
    src/editor_bindings/edi_jump_caret.cpp
    src/editor_bindings/edi_layout.cpp
    src/editor_bindings/edi_move_caret.cpp
    src/editor_bindings/edi_replace.cpp
    src/editor_bindings/edi_search_pattern.cpp
)
target_link_libraries(EdiBindings Common)
enable_cotire(EdiBindings)

add_library(GUIBindings STATIC
    src/gui_bindings/click.cpp
    src/gui_bindings/clipboard.cpp
    src/gui_bindings/easy_click.cpp
    src/gui_bindings/jump_cursor.cpp
    src/gui_bindings/move_cursor.cpp
    src/gui_bindings/pager.cpp
    src/gui_bindings/search_pattern.cpp
    src/gui_bindings/select.cpp
    src/gui_bindings/switch_vdesktop.cpp
)
target_link_libraries(GUIBindings Psapi Common)
enable_cotire(GUIBindings)

add_library(Options
    src/options/autotrack_popup.cpp
    src/options/dedicate_to_window.cpp
    src/options/virtual_cmd_line.cpp
)
target_link_libraries(Options Common)
enable_cotire(Options)

add_library(CoreSystem STATIC
    src/i_params.cpp
    src/key_binder.cpp
    src/msg_logger.cpp
    src/option_loader.cpp
    src/system.cpp
)
target_link_libraries(CoreSystem
    CmnBindings
    EdiBindings
    GUIBindings
    Options)
enable_cotire(CoreSystem)
