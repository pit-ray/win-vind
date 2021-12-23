#include <doctest.h>

#include "core/keycode.cpp"


TEST_CASE("KeyCode") {
    using namespace vind ;
    core::KeyCode key1('%') ;
    core::KeyCode key2(KEYCODE_SHIFT) ;
    core::KeyCode key3(KEYCODE_LSHIFT) ;
    core::KeyCode key4("shift") ;

    std::cout << key1 << std::endl ;
    std::cout << key2 << std::endl ;
    std::cout << key3 << std::endl ;
    std::cout << key4 << std::endl ;
}
