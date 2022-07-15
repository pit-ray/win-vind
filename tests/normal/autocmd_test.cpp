#include <doctest.h>

#include "core/autocmd.cpp"


TEST_SUITE("core/autocmd") {
    TEST_CASE("get_autocmd_event") {
        using namespace vind::core ;

        CHECK_EQ(get_autocmd_event("WinEnter"), AutoCmdEvent::WIN_ENTER) ;
        CHECK_THROWS(get_autocmd_event("----")) ;
    }
}
