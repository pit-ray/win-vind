cmake_minimum_required(VERSION 3.0.0)

set(SRCDIR ../../src)

AddTest(each-mapgate-test
    ${CMAKE_CURRENT_SOURCE_DIR}/mapgate_test.cpp

    ${SRCDIR}/bind/binded_func.cpp
    ${SRCDIR}/core/g_maps.cpp
    ${SRCDIR}/core/err_logger.cpp
    ${SRCDIR}/core/key_log.cpp
    ${SRCDIR}/core/key_absorber.cpp
    ${SRCDIR}/core/key_logger_base.cpp
    ${SRCDIR}/core/logger_parser.cpp
    ${SRCDIR}/core/logger_parser_mgr.cpp
    ${SRCDIR}/core/keycodecvt.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/ntype_logger.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/core/bindings_parser.cpp
    ${SRCDIR}/util/keybrd.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/winwrap.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/bind/emu/edi_dot.cpp
)
target_link_libraries(each-mapgate-test userenv)
