#include <doctest/doctest.h>

#include "core/typeemu.hpp"


namespace vind
{
    namespace bind
    {
        const std::string& BindedFunc::name() const noexcept {
            static std::string tmp = "<func>" ;
            return tmp ;
        }

        SystemCall BindedFunc::process() {
            return SystemCall::NOTHING ;
        }

        BindedFunc::SPtr ref_global_func_byname(const std::string& name) {
            return nullptr ;
        }
    }
}


TEST_SUITE("core/typeemu") {
    using namespace vind::core ;
    TEST_CASE("typing: tg") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_T} ;
        CmdUnit t2{KEYCODE_T} ;
        CmdUnit t3{KEYCODE_T, KEYCODE_G} ;
        CmdUnit t4{KEYCODE_T, KEYCODE_G} ;
        CmdUnit t5{KEYCODE_T, KEYCODE_G} ;
        CmdUnit t6{KEYCODE_G} ;
        CmdUnit t7{KEYCODE_G} ;
        CmdUnit t8{KEYCODE_G} ;

        CmdUnit e1{KEYCODE_T} ;
        CmdUnit e2{KEYCODE_G} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 != nullptr) ;
        CHECK_EQ(*r3, e2) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 == nullptr) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 == nullptr) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 == nullptr) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 == nullptr) ;

        auto r8 = typer.lowlevel_to_typing(t8) ;
        CHECK(r8 == nullptr) ;
    }

    TEST_CASE("typing: <s-f><s-f>") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t2{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t3{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t4{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;
        CmdUnit t5{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;
        CmdUnit t6{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;
        CmdUnit t7{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t8{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t9{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit t10{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;
        CmdUnit t11{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;

        CmdUnit e1{KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
        CmdUnit e2{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;
        CmdUnit e3{KEYCODE_SHIFT, KEYCODE_LSHIFT, KEYCODE_F} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 == nullptr) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 != nullptr) ;
        CHECK_EQ(*r4, e2) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 == nullptr) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 == nullptr) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 == nullptr) ;

        auto r8 = typer.lowlevel_to_typing(t8) ;
        CHECK(r8 == nullptr) ;

        auto r9 = typer.lowlevel_to_typing(t9) ;
        CHECK(r9 == nullptr) ;

        auto r10 = typer.lowlevel_to_typing(t10) ;
        CHECK(r10 != nullptr) ;
        CHECK_EQ(*r10, e3) ;

        auto r11 = typer.lowlevel_to_typing(t11) ;
        CHECK(r11 == nullptr) ;
    }

    TEST_CASE("typing: gg") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_G} ;
        CmdUnit t2{KEYCODE_G} ;
        CmdUnit t3{} ;
        CmdUnit t4{} ;
        CmdUnit t5{} ;
        CmdUnit t6{KEYCODE_G} ;
        CmdUnit t7{KEYCODE_G} ;

        CmdUnit e1{KEYCODE_G} ;
        CmdUnit e2{KEYCODE_G} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 == nullptr) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 == nullptr) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 == nullptr) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 != nullptr) ;
        CHECK_EQ(*r6, e2) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 == nullptr) ;
    }

    TEST_CASE("typing: <ctrl><ctrl>") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_CTRL} ;
        CmdUnit t2{KEYCODE_CTRL} ;
        CmdUnit t3{KEYCODE_CTRL} ;
        CmdUnit t4{} ;
        CmdUnit t5{} ;
        CmdUnit t6{} ;
        CmdUnit t7{KEYCODE_CTRL} ;
        CmdUnit t8{KEYCODE_CTRL} ;

        CmdUnit e1{KEYCODE_CTRL} ;
        CmdUnit e2{KEYCODE_CTRL} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 == nullptr) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 == nullptr) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 == nullptr) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 == nullptr) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 != nullptr) ;
        CHECK_EQ(*r7, e2) ;

        auto r8 = typer.lowlevel_to_typing(t8) ;
        CHECK(r8 == nullptr) ;
    }

    TEST_CASE("typing: <c-b>b") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_CTRL} ;
        CmdUnit t2{KEYCODE_CTRL} ;
        CmdUnit t3{KEYCODE_CTRL, KEYCODE_B} ;
        CmdUnit t4{KEYCODE_CTRL, KEYCODE_B} ;
        CmdUnit t5{KEYCODE_CTRL} ;
        CmdUnit t6{} ;
        CmdUnit t7{KEYCODE_B} ;
        CmdUnit t8{KEYCODE_B} ;

        CmdUnit e1{KEYCODE_CTRL} ;
        CmdUnit e2{KEYCODE_CTRL, KEYCODE_B} ;
        CmdUnit e3{KEYCODE_B} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 != nullptr) ;
        CHECK_EQ(*r3, e2) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 == nullptr) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 == nullptr) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 == nullptr) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 != nullptr) ;
        CHECK_EQ(*r7, e3) ;

        auto r8 = typer.lowlevel_to_typing(t8) ;
        CHECK(r8 == nullptr) ;
    }

    TEST_CASE("typing: hH") {
        TypingEmulator typer{} ;

        CmdUnit t1{KEYCODE_H} ;
        CmdUnit t2{KEYCODE_H} ;
        CmdUnit t3{} ;
        CmdUnit t4{} ;
        CmdUnit t5{KEYCODE_SHIFT} ;
        CmdUnit t6{KEYCODE_SHIFT} ;
        CmdUnit t7{KEYCODE_SHIFT, KEYCODE_H} ;
        CmdUnit t8{KEYCODE_SHIFT, KEYCODE_H} ;

        CmdUnit e1{KEYCODE_H} ;
        CmdUnit e2{KEYCODE_SHIFT} ;
        CmdUnit e3{KEYCODE_SHIFT, KEYCODE_H} ;

        auto r1 = typer.lowlevel_to_typing(t1) ;
        CHECK(r1 != nullptr) ;
        CHECK_EQ(*r1, e1) ;

        auto r2 = typer.lowlevel_to_typing(t2) ;
        CHECK(r2 == nullptr) ;

        auto r3 = typer.lowlevel_to_typing(t3) ;
        CHECK(r3 == nullptr) ;

        auto r4 = typer.lowlevel_to_typing(t4) ;
        CHECK(r4 == nullptr) ;

        auto r5 = typer.lowlevel_to_typing(t5) ;
        CHECK(r5 != nullptr) ;
        CHECK_EQ(*r5, e2) ;

        auto r6 = typer.lowlevel_to_typing(t6) ;
        CHECK(r6 == nullptr) ;

        auto r7 = typer.lowlevel_to_typing(t7) ;
        CHECK(r7 != nullptr) ;
        CHECK_EQ(*r7, e3) ;

        auto r8 = typer.lowlevel_to_typing(t8) ;
        CHECK(r8 == nullptr) ;
    }
}

