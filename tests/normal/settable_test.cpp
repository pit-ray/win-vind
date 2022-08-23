#include <doctest/doctest.h>

#include "core/settable.cpp"

#include <iostream>


TEST_SUITE("core/settable") {
    TEST_CASE("Param") {
        using namespace vind::core ;
        Param a("Foo", false) ;
        Param b("Bar", 25) ;
        Param c("BarBar", "gh") ;

        SUBCASE("Param::name") {
            CHECK_EQ(a.name(), "Foo") ;
            CHECK_EQ(b.name(), "Bar") ;
            CHECK_EQ(c.name(), "BarBar") ;
        }

        SUBCASE("Param::is_bool") {
            CHECK(a.is_bool()) ;
            CHECK_FALSE(b.is_bool()) ;
            CHECK_FALSE(c.is_bool()) ;
        }

        SUBCASE("Param::is_string") {
            CHECK_FALSE(a.is_string()) ;
            CHECK_FALSE(b.is_string()) ;
            CHECK(c.is_string()) ;
        }

        SUBCASE("Param::is_number") {
            CHECK_FALSE(a.is_number()) ;
            CHECK(b.is_number()) ;
            CHECK_FALSE(c.is_number()) ;
        }

        SUBCASE("Param::get") {
            CHECK_EQ(a.get<bool>(), false) ;
            CHECK_EQ(b.get<long>(), 25) ;
            CHECK_EQ(c.get<std::string>(), "gh") ;
        }
    }


    TEST_CASE("SetTable") {
        using namespace vind::core ;
        auto& settable = SetTable::get_instance() ;

        settable.clear() ;
        SUBCASE("I/O conversion") {
            settable.set("A", true) ;
            const auto& a = settable.get("A") ;
            CHECK_EQ(a.get<bool>(), true) ;
            CHECK_EQ(a.get<char>(), 1) ;
            CHECK_EQ(a.get<unsigned char>(), 1) ;
            CHECK_EQ(a.get<short>(), 1) ;
            CHECK_EQ(a.get<int>(), 1) ;
            CHECK_EQ(a.get<long>(), 1) ;
            CHECK_EQ(a.get<float>(), 1.0f) ;
            CHECK_EQ(a.get<double>(), 1.0) ;
            CHECK_EQ(a.get<std::string>(), "1") ;

            settable.set("B", 10) ;
            const auto& b = settable.get("B") ;
            CHECK_EQ(b.get<bool>(), true) ;
            CHECK_EQ(b.get<char>(), 10) ;
            CHECK_EQ(b.get<unsigned char>(), 10) ;
            CHECK_EQ(b.get<short>(), 10) ;
            CHECK_EQ(b.get<int>(), 10) ;
            CHECK_EQ(b.get<long>(), 10) ;
            CHECK_EQ(b.get<float>(), 10.0f) ;
            CHECK_EQ(b.get<double>(), 10.0) ;
            CHECK_EQ(b.get<std::string>(), "10") ;

            settable.set("C", 20.5f) ;
            const auto& c = settable.get("C") ;
            CHECK_EQ(c.get<bool>(), true) ;
            CHECK_EQ(c.get<char>(), 20) ;
            CHECK_EQ(c.get<unsigned char>(), 20) ;
            CHECK_EQ(c.get<short>(), 20) ;
            CHECK_EQ(c.get<int>(), 20) ;
            CHECK_EQ(c.get<long>(), 20) ;
            CHECK_EQ(c.get<float>(), 20.5f) ;
            CHECK_EQ(c.get<double>(), 20.5) ;
            CHECK_EQ(c.get<std::string>(), "20.500000") ;

            settable.set("D", "50.96") ;
            const auto& d = settable.get("D") ;
            CHECK_EQ(d.get<bool>(), true) ;
            CHECK_EQ(d.get<char>(), 50) ;
            CHECK_EQ(d.get<unsigned char>(), 50) ;
            CHECK_EQ(d.get<short>(), 50) ;
            CHECK_EQ(d.get<int>(), 50) ;
            CHECK_EQ(d.get<long>(), 50) ;
            CHECK_EQ(d.get<float>(), 50.96f) ;
            CHECK_EQ(d.get<double>(), 50.96) ;
            CHECK_EQ(d.get<std::string>(), "50.96") ;

            settable.set("E", "79") ;
            const auto& e = settable.get("E") ;
            CHECK_EQ(e.get<bool>(), true) ;
            CHECK_EQ(e.get<char>(), 79) ;
            CHECK_EQ(e.get<unsigned char>(), 79) ;
            CHECK_EQ(e.get<short>(), 79) ;
            CHECK_EQ(e.get<int>(), 79) ;
            CHECK_EQ(e.get<long>(), 79) ;
            CHECK_EQ(e.get<float>(), 79.0f) ;
            CHECK_EQ(e.get<double>(), 79.0) ;
            CHECK_EQ(e.get<std::string>(), "79") ;

            settable.set("F", "hello") ;
            const auto& f = settable.get("F") ;
            CHECK_EQ(f.get<bool>(), true) ;
            CHECK_THROWS(f.get<char>()) ;
            CHECK_THROWS(f.get<unsigned char>()) ;
            CHECK_THROWS(f.get<short>()) ;
            CHECK_THROWS(f.get<int>()) ;
            CHECK_THROWS(f.get<long>()) ;
            CHECK_THROWS(f.get<float>()) ;
            CHECK_THROWS(f.get<double>()) ;
            CHECK_EQ(f.get<std::string>(), "hello") ;

            settable.set("G", 0xffff'8f9f) ;
            const auto& g = settable.get("G") ;
            CHECK_EQ(g.get<bool>(), true) ;
            CHECK_EQ(g.get<char>(), -97) ;
            CHECK_EQ(g.get<unsigned char>(), 159) ;
            CHECK_EQ(g.get<short>(), -28769) ;
            CHECK_EQ(g.get<unsigned short>(), 36767) ;
            CHECK_EQ(g.get<int>(), -28769) ;
            CHECK_EQ(g.get<unsigned int>(), 4294938527) ;
            CHECK_EQ(g.get<long>(), static_cast<long>(0xffff'8f9f)) ;
            CHECK_EQ(g.get<float>(), -28769.0f) ;
            CHECK_EQ(g.get<double>(), -28769.0f) ;
            CHECK_EQ(g.get<std::string>(), std::to_string(static_cast<long>(0xffff'8f9f))) ;
        }
    }
}
