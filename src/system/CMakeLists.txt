cmake_minimum_required(VERSION 3.0.0)

set(SYSINCDIR ../../include/system)
include_directories(
    ${SYSINCDIR}
    ${SYSINCDIR}/common
    ${SYSINCDIR}/keybinds
    ${SYSINCDIR}/editor_keybinds
    ${SYSINCDIR}/options
    C:/boost
)

add_library(Common STATIC
    common/binded_function.cpp
    common/command.cpp
    common/dynamic_option.cpp
    common/interval_timer.cpp
    common/key_absorber.cpp
    common/key_log.cpp
    common/key_logger.cpp
    common/keybrd_eventer.cpp
    common/mode_manager.cpp
    common/mouse_eventer.cpp
    common/text_analyzer.cpp
    common/simpl_text_selecter.cpp
    common/utility.cpp
    common/vkc_converter.cpp
    common/xml_parser.cpp
    common/dynamic_config.cpp
)

add_library(EditorKeybinds STATIC
    editor_keybinds/edi_change_mode.cpp
    editor_keybinds/edi_move_caret.cpp
    editor_keybinds/edi_jump_caret.cpp
    editor_keybinds/edi_search_pattern.cpp
    editor_keybinds/edi_clipboard.cpp
    editor_keybinds/edi_replace.cpp
    editor_keybinds/edi_layout.cpp
)
target_link_libraries(EditorKeybinds Common)

add_library(Keybinds STATIC
    keybinds/change_mode.cpp
    keybinds/click.cpp
    keybinds/clipboard.cpp
    keybinds/external_app.cpp
    keybinds/filer.cpp
    keybinds/jump_cursor.cpp
    keybinds/move_cursor.cpp
    keybinds/pager.cpp
    keybinds/scroll.cpp
    keybinds/search_pattern.cpp
    keybinds/select.cpp
    keybinds/switch_vdesktop.cpp
    keybinds/undo.cpp
    keybinds/window_ctrl.cpp
    keybinds/mywindow_ctrl.cpp
)
target_link_libraries(Keybinds Common Psapi)

add_library(Options
    options/autotrack_popup.cpp
    options/display_cmd.cpp
)
target_link_libraries(Options Common)

add_library(Binders STATIC
    key_binder.cpp
    option_loader.cpp
)
target_link_libraries(Binders EditorKeybinds Keybinds Options)

add_library(CoreSystem STATIC
    system.cpp
)
target_link_libraries(CoreSystem Binders)