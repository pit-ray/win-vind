#include <doctest.h>

#include "core/mapgate.cpp"

using namespace vind ;
using namespace vind::core ;

namespace vind
{
    namespace bind
    {
        const auto& all_global_binded_funcs() {
            static std::vector<BindedFunc::SPtr> tmp {
            } ;
            return tmp ;
        }
    }
}


TEST_CASE("mapgate::solve_recursive_key2keyset_mapping") {
    Key2KeysetMap table ;

    KeyUnorderedSet target_a = {KEYCODE_C, KEYCODE_D} ; 
    table[KEYCODE_A] = target_a ;

    KeyUnorderedSet target_d = {KEYCODE_G, KEYCODE_E} ;
    table[KEYCODE_D] = target_d ;

    solve_recursive_key2keyset_mapping(table) ;

    KeyUnorderedSet expect_a = {KEYCODE_C, KEYCODE_G, KEYCODE_E} ;
    CHECK_EQ(table[KEYCODE_A], expect_a) ;

    KeyUnorderedSet expect_d = {KEYCODE_G, KEYCODE_E} ;
    CHECK_EQ(table[KEYCODE_D], expect_d) ;
}
