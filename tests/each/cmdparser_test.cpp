#include <doctest/doctest.h>

#include "core/cmdparser.hpp"
#include "core/keycodedef.hpp"

#include <iostream>
#include <memory>

namespace vind
{
    namespace bind
    {
        SystemCall BindedFunc::process(std::uint16_t, const std::string&) {
            return SystemCall::NOTHING ;
        }

        BindedFunc::SPtr ref_global_func_byname(const std::string& name) {
            if(name == "easy_click_left") {
                return std::make_unique<BindedFunc>("easy_click_left") ;
            }
            if(name == "hogehoge") {
                return std::make_unique<BindedFunc>("hogehoge") ;
            }
            return nullptr ;
        }

        BindedFunc::SPtr ref_global_funcs_bynames(const std::string& name) {
            return nullptr ;
        }
    }
}

namespace
{
    using namespace vind::core ;

    enum class UnitScope : unsigned char {
        INTERNAL,
        EXTERNAL,
        FUNCTIONAL
    } ;

    bool check_scope(const CmdUnit::SPtr& ptr, UnitScope expect_scope) {
        auto i_ptr = std::dynamic_pointer_cast<InternalCmdUnit>(ptr) ;
        auto e_ptr = std::dynamic_pointer_cast<ExternalCmdUnit>(ptr) ;
        auto f_ptr = std::dynamic_pointer_cast<FunctionalCmdUnit>(ptr) ;

        switch(expect_scope) {
            case UnitScope::INTERNAL:
                return i_ptr && !e_ptr && !f_ptr ;

            case UnitScope::EXTERNAL:
                return !i_ptr && e_ptr && !f_ptr ;

            case UnitScope::FUNCTIONAL:
                return !i_ptr && !e_ptr && f_ptr ;
        }

        return false ;
    }
}


TEST_SUITE("core/cmdparser") {
    using namespace vind::core ;
    TEST_CASE("parse_ascii_command") {
        SUBCASE("case1") {
            auto ks1 = parse_ascii_command('A') ;
            CmdUnitSet expect1{KEYCODE_SHIFT, KEYCODE_A} ;
            CHECK_EQ(ks1, expect1) ;
        }

        SUBCASE("case2") {
            auto ks2 = parse_ascii_command('x') ;
            CmdUnitSet expect2{KEYCODE_X} ;
            CHECK_EQ(ks2, expect2) ;
        }

        SUBCASE("case3") {
            CHECK_THROWS(parse_ascii_command(0)) ;
        }
    }

    TEST_CASE("parse_combined_command") {
        SUBCASE("case1") {
            auto ks1 = parse_combined_command("s-c-h-j") ;
            CmdUnitSet expect1 = {KEYCODE_SHIFT, KEYCODE_C, KEYCODE_H, KEYCODE_J} ;
            CHECK_EQ(ks1, expect1) ;
        }

        SUBCASE("case2") {
            auto ks2 = parse_combined_command("shift-ctrl-X-b-a") ;
            CmdUnitSet expect2 = {KEYCODE_CTRL, KEYCODE_SHIFT, KEYCODE_A, KEYCODE_B, KEYCODE_X} ;
            CHECK_EQ(ks2, expect2) ;
        }

        SUBCASE("case3") {
            auto ks3 = parse_combined_command("ctrl-p-N-alt") ;
            CmdUnitSet expect3 = {KEYCODE_CTRL, KEYCODE_ALT, KEYCODE_SHIFT, KEYCODE_N, KEYCODE_P} ;
            CHECK_EQ(ks3, expect3) ;
        }

        SUBCASE("case4") {
            auto ks4 = parse_combined_command("a-a") ;
            CmdUnitSet expect4 = {KEYCODE_ALT, KEYCODE_A} ;
            CHECK_EQ(ks4, expect4) ;
        }

        SUBCASE("case5") {
            CHECK_THROWS(parse_combined_command("j-b")) ;
        }

        SUBCASE("case6") {
            CHECK_THROWS(parse_combined_command("shift-fjakls")) ;
        }
    }

    TEST_CASE("parse_command") {
        SUBCASE("case1") {
            auto cmd1 = parse_command("<s-h>jH<alt-b><m-m>") ;
            std::vector<CmdUnitSet> expect1 {
                {KEYCODE_SHIFT, KEYCODE_H},
                {KEYCODE_J},
                {KEYCODE_SHIFT, KEYCODE_H},
                {KEYCODE_ALT, KEYCODE_B},
                {KEYCODE_ALT, KEYCODE_M}
            } ;
            CHECK_EQ(cmd1.size(), expect1.size()) ;
            for(std::size_t i = 0 ; i < expect1.size() ; i ++) {
                CHECK_NE(
                    std::dynamic_pointer_cast<InternalCmdUnit>(cmd1[i]),
                    nullptr) ;

                CHECK_EQ(*cmd1[i], expect1[i]) ;
            }
        }

        SUBCASE("case2") {
            auto cmd2 = parse_command("<Esc-Left>") ;
            std::vector<CmdUnitSet> expect2 {
                {KEYCODE_ESC, KEYCODE_LEFT}
            } ;
            CHECK_EQ(cmd2.size(), expect2.size()) ;
            for(std::size_t i = 0 ; i < expect2.size() ; i ++) {
                CHECK_NE(
                    std::dynamic_pointer_cast<InternalCmdUnit>(cmd2[i]),
                    nullptr) ;

                CHECK_EQ(*cmd2[i], expect2[i]) ;
            }
        }

        SUBCASE("case3") {
            CmdUnitSet ks3{} ;
            KeyCode k3('!') ;
            if(auto shift = get_shift_keycode('!')) {
                ks3.insert(shift) ;
            }
            ks3.insert(k3) ;

            std::vector<CmdUnitSet> expect3 {
                ks3,
                {KEYCODE_OPTIONAL}
            } ;
            auto cmd3 = parse_command("!<any>") ;
            CHECK_EQ(cmd3.size(), expect3.size()) ;
            for(std::size_t i = 0 ; i < expect3.size() ; i ++) {
                CHECK_NE(
                    std::dynamic_pointer_cast<InternalCmdUnit>(cmd3[i]),
                    nullptr) ;

                CHECK_EQ(*cmd3[i], expect3[i]) ;
            }
        }

        SUBCASE("case4") {
            CHECK_THROWS(parse_command("b<shift-j><a-jmh")) ;
        }

        SUBCASE("case5") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_SHIFT, KEYCODE_J},
                {KEYCODE_CTRL, KEYCODE_C},
                {KEYCODE_J},
                {KEYCODE_R},
            } ;

            std::vector<UnitScope> scope {
                UnitScope::INTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::INTERNAL,
            } ;

            auto cmd = parse_command("<s-j>{<c-c>}{j}r") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }

        SUBCASE("case6") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_F, KEYCODE_CTRL},
                {KEYCODE_H},
                {KEYCODE_A},
                {KEYCODE_J},
                {KEYCODE_SHIFT, KEYCODE_ALT, KEYCODE_B},
            } ;

            std::vector<UnitScope> scope {
                UnitScope::INTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
            } ;

            auto cmd = parse_command("<c-f>{haj<shift-alt-b>}") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }

        SUBCASE("case7") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_A},
                {KEYCODE_CTRL, KEYCODE_J},
                parse_ascii_command('{'),
                {KEYCODE_J},
                {KEYCODE_K},
                parse_ascii_command('['),
                {KEYCODE_J},
                parse_ascii_command('{'),
                {KEYCODE_B},
                {KEYCODE_F},
                {KEYCODE_K},
                parse_ascii_command('}')
            } ;

            std::vector<UnitScope> scope {
                UnitScope::INTERNAL,
                UnitScope::INTERNAL,

                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,

                UnitScope::INTERNAL,

                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,

                UnitScope::INTERNAL,
                UnitScope::INTERNAL,
                UnitScope::INTERNAL,
            } ;

            auto cmd = parse_command("a<c-j>{{jk}[{j{b}fk}") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }

        SUBCASE("case7") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_A},
                {KEYCODE_CTRL, KEYCODE_J},
                parse_ascii_command('{'),
                {KEYCODE_J},
                {KEYCODE_K},
                parse_ascii_command('['),
                {KEYCODE_J},
                parse_ascii_command('{'),
                {KEYCODE_B},
                {KEYCODE_F},
                {KEYCODE_K},
                parse_ascii_command('}')
            } ;

            std::vector<UnitScope> scope {
                UnitScope::INTERNAL,
                UnitScope::INTERNAL,

                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,

                UnitScope::INTERNAL,

                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,
                UnitScope::EXTERNAL,

                UnitScope::INTERNAL,
                UnitScope::INTERNAL,
                UnitScope::INTERNAL,
            } ;

            auto cmd = parse_command("a<c-j>{{jk}[{j{b}fk}") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }

        SUBCASE("case8") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_CTRL, KEYCODE_S},
                {KEYCODE_A},
                {},
                {KEYCODE_B}
            } ;

            std::vector<UnitScope> scope {
                UnitScope::INTERNAL,
                UnitScope::INTERNAL,
                UnitScope::FUNCTIONAL,
                UnitScope::EXTERNAL
            } ;

            auto cmd = parse_command("<c-s>a<easy_click_left>{b}") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }

        SUBCASE("case9") {
            CHECK_THROWS(parse_command("<c-s><bad_function_name>{b}")) ;
        }

        SUBCASE("case10") {
            std::vector<CmdUnitSet> expect {
                {KEYCODE_G},
            } ;

            std::vector<UnitScope> scope {
                UnitScope::EXTERNAL
            } ;

            auto cmd = parse_command("{g}") ;
            CHECK_EQ(cmd.size(), expect.size()) ;

            for(std::size_t i = 0 ; i < expect.size() ; i ++) {
                CHECK(check_scope(cmd[i], scope[i])) ;
                CHECK_EQ(*cmd[i], expect[i]) ;
            }
        }
    }
}
