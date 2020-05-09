cmake_minimum_required(VERSION 3.0.0)

include_directories(
    ../include
    ../include/keybinds
    ../include/editor_keybinds
    ../include/options
    C:/boost
)

#Common
add_library(Common STATIC
    utility.cpp
    mode_manager.cpp
    key_absorber.cpp
    interval_timer.cpp
    xml_parser.cpp
    text_analyzer.cpp
    mouse_eventer.cpp
    keybrd_eventer.cpp
    key_log.cpp
    key_logger.cpp
    binded_function.cpp
    command.cpp
    dynamic_option.cpp
    vkc_converter.cpp
)
target_link_libraries(Common Imm32.lib)

#keybinds
set(LIB1_DIR keybinds)
add_library(Keybinds STATIC
    ${LIB1_DIR}/move_cursor.cpp
    ${LIB1_DIR}/jump_cursor.cpp
    ${LIB1_DIR}/scroll.cpp
    ${LIB1_DIR}/click.cpp
    ${LIB1_DIR}/change_mode.cpp
    ${LIB1_DIR}/undo.cpp
    ${LIB1_DIR}/clipboard.cpp
    ${LIB1_DIR}/switch_taskview.cpp
    ${LIB1_DIR}/search_pattern.cpp
    ${LIB1_DIR}/select.cpp
    ${LIB1_DIR}/window_ctrl.cpp
    ${LIB1_DIR}/filer.cpp
    ${LIB1_DIR}/pager.cpp
    ${LIB1_DIR}/external_app.cpp
)
target_link_libraries(Keybinds Common)

#EditorKeybinds
set(LIB2_DIR editor_keybinds)
add_library(EditorKeybinds STATIC
    ${LIB2_DIR}/edi_move_cursor.cpp
    ${LIB2_DIR}/edi_change_mode.cpp
)
target_link_libraries(EditorKeybinds Common)

#Options
set(LIB3_DIR options)
add_library(Options STATIC
    ${LIB3_DIR}/autotrack_popup.cpp
    ${LIB3_DIR}/display_cmd.cpp
)
target_link_libraries(Options Common)

#final
add_library(Main STATIC
    key_binder.cpp
    option_loader.cpp
    system.cpp
)
target_link_libraries(Main Keybinds EditorKeybinds Options)