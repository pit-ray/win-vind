cmake_minimum_required(VERSION 3.0.0)

set(SYSINCDIR ../../include/system)
include_directories(
    ${SYSINCDIR}
    ${SYSINCDIR}/common
    ${SYSINCDIR}/gui_bindings
    ${SYSINCDIR}/editor_bindings
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

add_library(EditorBindings STATIC
    editor_bindings/edi_change_mode.cpp
    editor_bindings/edi_move_caret.cpp
    editor_bindings/edi_jump_caret.cpp
    editor_bindings/edi_search_pattern.cpp
    editor_bindings/edi_clipboard.cpp
    editor_bindings/edi_replace.cpp
    editor_bindings/edi_layout.cpp
)
target_link_libraries(EditorBindings Common)

add_library(GUIBindings STATIC
    gui_bindings/change_mode.cpp
    gui_bindings/click.cpp
    gui_bindings/clipboard.cpp
    gui_bindings/external_app.cpp
    gui_bindings/filer.cpp
    gui_bindings/jump_cursor.cpp
    gui_bindings/move_cursor.cpp
    gui_bindings/pager.cpp
    gui_bindings/scroll.cpp
    gui_bindings/search_pattern.cpp
    gui_bindings/select.cpp
    gui_bindings/switch_vdesktop.cpp
    gui_bindings/undo.cpp
    gui_bindings/window_ctrl.cpp
    gui_bindings/mywindow_ctrl.cpp
)
target_link_libraries(GUIBindings Common Psapi)

add_library(Options
    options/autotrack_popup.cpp
    options/display_cmd.cpp
    options/dedicate_to_window.cpp
)
target_link_libraries(Options Common)

add_library(Binders STATIC
    key_binder.cpp
    option_loader.cpp
)
target_link_libraries(Binders EditorBindings GUIBindings Options)

add_library(CoreSystem STATIC
    system.cpp
)
target_link_libraries(CoreSystem Binders)