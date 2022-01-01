cmake_minimum_required(VERSION 3.0.0)

set(SRCDIR ../../src)

# AddTest(each-mapgate-test
#     ${CMAKE_CURRENT_SOURCE_DIR}/inputgate_test.cpp
# 
#     ${SRCDIR}/bind/bindedfunc.cpp
#     ${SRCDIR}/core/cmdparser.cpp
#     ${SRCDIR}/core/errlogger.cpp
#     ${SRCDIR}/core/keylog.cpp
#     ${SRCDIR}/core/keylgrbase.cpp
#     ${SRCDIR}/core/keycode.cpp
#     ${SRCDIR}/core/lgrparser.cpp
#     ${SRCDIR}/core/lgrparsermgr.cpp
#     ${SRCDIR}/core/maptable.cpp
#     ${SRCDIR}/core/mode.cpp
#     ${SRCDIR}/core/ntypelogger.cpp
#     ${SRCDIR}/core/path.cpp
#     ${SRCDIR}/util/interval_timer.cpp
#     ${SRCDIR}/util/keystroke_repeater.cpp
#     ${SRCDIR}/util/string.cpp
#     ${SRCDIR}/util/winwrap.cpp
# )
# target_link_libraries(each-mapgate-test userenv)

AddTest(each-maptable-test
    ${CMAKE_CURRENT_SOURCE_DIR}/maptable_test.cpp

    ${SRCDIR}/core/cmdparser.cpp
    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/keylog.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-maptable-test userenv)

# AddTest(each-cmdparser-test
#     ${CMAKE_CURRENT_SOURCE_DIR}/cmdparser_test.cpp
# 
#     ${SRCDIR}/util/string.cpp
#     ${SRCDIR}/core/cmdparser.cpp
#     ${SRCDIR}/core/errlogger.cpp
#     ${SRCDIR}/core/keycode.cpp
#     ${SRCDIR}/core/path.cpp
#     ${SRCDIR}/util/winwrap.cpp
# )
# target_link_libraries(each-cmdparser-test userenv)
