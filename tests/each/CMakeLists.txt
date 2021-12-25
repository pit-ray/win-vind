cmake_minimum_required(VERSION 3.0.0)

set(SRCDIR ../../src)

AddTest(each-mapgate-test
    ${CMAKE_CURRENT_SOURCE_DIR}/inputgate_test.cpp

    ${SRCDIR}/bind/binded_func.cpp
    ${SRCDIR}/core/bindings_parser.cpp
    ${SRCDIR}/core/err_logger.cpp
    ${SRCDIR}/core/key_log.cpp
    ${SRCDIR}/core/key_logger_base.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/logger_parser.cpp
    ${SRCDIR}/core/logger_parser_mgr.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/ntype_logger.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-mapgate-test userenv)

AddTest(each-maptable-test
    ${CMAKE_CURRENT_SOURCE_DIR}/maptable_test.cpp

    ${SRCDIR}/core/bindings_parser.cpp
    ${SRCDIR}/core/err_logger.cpp
    ${SRCDIR}/core/key_log.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-maptable-test userenv)

AddTest(each-bindings-parser-test
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_parser_test.cpp

    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/core/bindings_parser.cpp
    ${SRCDIR}/core/err_logger.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-bindings-parser-test userenv)
