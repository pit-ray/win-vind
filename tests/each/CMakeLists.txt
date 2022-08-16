cmake_minimum_required(VERSION 3.0.0)

set(SRCDIR ../../src)

AddTest(each-mapgate-test
    ${CMAKE_CURRENT_SOURCE_DIR}/inputgate_test.cpp

    ${SRCDIR}/bind/bindedfunc.cpp
    ${SRCDIR}/core/cmdparser_old.cpp
    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/keylgrbase.cpp
    ${SRCDIR}/core/keylog.cpp
    ${SRCDIR}/core/lgrparser.cpp
    ${SRCDIR}/core/lgrparsermgr.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/ntypelogger.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/box2d.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-mapgate-test userenv)

AddTest(each-maptable-test
    ${CMAKE_CURRENT_SOURCE_DIR}/maptable_test.cpp

    ${SRCDIR}/core/cmdparser_old.cpp
    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/keylog.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/box2d.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-maptable-test userenv)

AddTest(each-cmdparser-old-test
    ${CMAKE_CURRENT_SOURCE_DIR}/cmdparser_old_test.cpp

    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/core/cmdparser_old.cpp
    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/box2d.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-cmdparser-old-test userenv)


AddTest(each-cmdparser-test
    ${CMAKE_CURRENT_SOURCE_DIR}/cmdparser_test.cpp

    ${SRCDIR}/bind/bindedfunc.cpp
    ${SRCDIR}/core/cmdparser.cpp
    ${SRCDIR}/core/cmdparser_old.cpp
    ${SRCDIR}/core/cmdunit.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/util/string.cpp

    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/inputgate.cpp
    ${SRCDIR}/core/keylgrbase.cpp
    ${SRCDIR}/core/keylog.cpp
    ${SRCDIR}/core/lgrparser.cpp
    ${SRCDIR}/core/lgrparsermgr.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/ntypelogger.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/box2d.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-cmdparser-test userenv)


AddTest(each-cmdmatcher-test
    ${CMAKE_CURRENT_SOURCE_DIR}/cmdmatcher_test.cpp

    ${SRCDIR}/core/cmdmatcher.cpp
    ${SRCDIR}/core/cmdunit.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/util/string.cpp
)


AddTest(each-mapsolver-test
    ${CMAKE_CURRENT_SOURCE_DIR}/mapsolver_test.cpp

    ${SRCDIR}/bind/bindedfunc.cpp
    ${SRCDIR}/core/cmdmatcher.cpp
    ${SRCDIR}/core/cmdparser.cpp
    ${SRCDIR}/core/cmdparser_old.cpp
    ${SRCDIR}/core/cmdunit.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/mapsolver.cpp
    ${SRCDIR}/util/string.cpp
    ${SRCDIR}/core/typeemu.cpp

    ${SRCDIR}/core/errlogger.cpp
    ${SRCDIR}/core/inputgate.cpp
    ${SRCDIR}/core/keylgrbase.cpp
    ${SRCDIR}/core/keylog.cpp
    ${SRCDIR}/core/lgrparser.cpp
    ${SRCDIR}/core/lgrparsermgr.cpp
    ${SRCDIR}/core/maptable.cpp
    ${SRCDIR}/core/mode.cpp
    ${SRCDIR}/core/ntypelogger.cpp
    ${SRCDIR}/core/path.cpp
    ${SRCDIR}/util/box2d.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/winwrap.cpp
)
target_link_libraries(each-mapsolver-test userenv)


AddTest(each-typemu-test
    ${CMAKE_CURRENT_SOURCE_DIR}/typeemu_test.cpp

    ${SRCDIR}/core/cmdunit.cpp
    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/core/typeemu.cpp
    ${SRCDIR}/util/interval_timer.cpp
    ${SRCDIR}/util/keystroke_repeater.cpp
    ${SRCDIR}/util/string.cpp
)


AddTest(each-keycode-test
    ${CMAKE_CURRENT_SOURCE_DIR}/keycode_test.cpp

    ${SRCDIR}/core/keycode.cpp
    ${SRCDIR}/util/string.cpp
)

AddTest(each-unicode-test
    ${CMAKE_CURRENT_SOURCE_DIR}/unicode_test.cpp
)
target_link_libraries(each-unicode-test icuuc)
