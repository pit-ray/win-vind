cmake_minimum_required(VERSION 3.0.0)

set(SRCDIR ${ROOT_DIR}/src)

AddTest(mock-string-test
    ${CMAKE_CURRENT_SOURCE_DIR}/string_test.cpp
)

AddTest(mock-winwrap-test
    ${CMAKE_CURRENT_SOURCE_DIR}/winwrap_test.cpp
    ${SRCDIR}/util/box2d.cpp
)

AddTest(mock-errlogger-test
    ${CMAKE_CURRENT_SOURCE_DIR}/errlogger_test.cpp
)
