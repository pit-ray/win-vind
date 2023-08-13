#include "core/autocmd.cpp"

#include <doctest/doctest.h>
#include <unordered_set>

namespace vind
{
    namespace bind
    {
        BindedFunc::SPtr search_func(const std::string&) {
            return nullptr ;
        }
    }
}


TEST_CASE("get_autocmd_event") {
    using namespace core ;
    CHECK_EQ(get_autocmd_event("AppEnter"), AutoCmdEvent::APP_ENTER) ;
    CHECK_EQ(get_autocmd_event("AppLeave"), AutoCmdEvent::APP_LEAVE) ;
    CHECK_EQ(get_autocmd_event("GUINormalEnter"), AutoCmdEvent::GUI_NORMAL_ENTER) ;
    CHECK_EQ(get_autocmd_event("GUINormalLeave"), AutoCmdEvent::GUI_NORMAL_LEAVE) ;
    CHECK_EQ(get_autocmd_event("EdiNormalEnter"), AutoCmdEvent::EDI_NORMAL_ENTER) ;
    CHECK_EQ(get_autocmd_event("EdiNormalLeave"), AutoCmdEvent::EDI_NORMAL_LEAVE) ;
    CHECK_EQ(get_autocmd_event("GUIVisualEnter"), AutoCmdEvent::GUI_VISUAL_ENTER) ;
    CHECK_EQ(get_autocmd_event("GUIVisualLeave"), AutoCmdEvent::GUI_VISUAL_LEAVE) ;
    CHECK_EQ(get_autocmd_event("EdiVisualEnter"), AutoCmdEvent::EDI_VISUAL_ENTER) ;
    CHECK_EQ(get_autocmd_event("EdiVisualLeave"), AutoCmdEvent::EDI_VISUAL_LEAVE) ;
    CHECK_EQ(get_autocmd_event("InsertEnter"), AutoCmdEvent::INSERT_ENTER) ;
    CHECK_EQ(get_autocmd_event("InsertLeave"), AutoCmdEvent::INSERT_LEAVE) ;
    CHECK_EQ(get_autocmd_event("ResidentEnter"), AutoCmdEvent::RESIDENT_ENTER) ;
    CHECK_EQ(get_autocmd_event("ResidentLeave"), AutoCmdEvent::RESIDENT_LEAVE) ;
    CHECK_EQ(get_autocmd_event("CommandEnter"), AutoCmdEvent::COMMAND_ENTER) ;
    CHECK_EQ(get_autocmd_event("CommandLeave"), AutoCmdEvent::COMMAND_LEAVE) ;
    CHECK_EQ(get_autocmd_event("hjkl"), AutoCmdEvent::UNDEFINED) ;
}

TEST_CASE("get_enter_event") {
    using namespace core ;

    CHECK_EQ(get_enter_event(Mode::GUI_NORMAL), AutoCmdEvent::GUI_NORMAL_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::EDI_NORMAL), AutoCmdEvent::EDI_NORMAL_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::GUI_VISUAL), AutoCmdEvent::GUI_VISUAL_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::EDI_VISUAL), AutoCmdEvent::EDI_VISUAL_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::INSERT), AutoCmdEvent::INSERT_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::RESIDENT), AutoCmdEvent::RESIDENT_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::COMMAND), AutoCmdEvent::COMMAND_ENTER) ;
    CHECK_EQ(get_enter_event(Mode::UNDEFINED), AutoCmdEvent::UNDEFINED) ;
}

TEST_CASE("get_leave_event") {
    using namespace core ;

    CHECK_EQ(get_leave_event(Mode::GUI_NORMAL), AutoCmdEvent::GUI_NORMAL_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::EDI_NORMAL), AutoCmdEvent::EDI_NORMAL_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::GUI_VISUAL), AutoCmdEvent::GUI_VISUAL_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::EDI_VISUAL), AutoCmdEvent::EDI_VISUAL_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::INSERT), AutoCmdEvent::INSERT_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::RESIDENT), AutoCmdEvent::RESIDENT_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::COMMAND), AutoCmdEvent::COMMAND_LEAVE) ;
    CHECK_EQ(get_leave_event(Mode::UNDEFINED), AutoCmdEvent::UNDEFINED) ;
}

TEST_CASE("AutoPattern") {
    SUBCASE("Simple") {
        AutoPattern ap1("*") ;
        AutoPattern ap2("?") ;
        AutoPattern ap3("\\?") ;
        AutoPattern ap4(".") ;
        AutoPattern ap5("~") ;
        AutoPattern ap6("[ch]") ;
        AutoPattern ap7("[^ch]") ;

        CHECK(ap1.match("abcd")) ;
        CHECK(ap1.match("a")) ;

        CHECK(ap2.match("x")) ;
        CHECK(ap2.match("2")) ;
        CHECK_FALSE(ap2.match("ajb")) ;

        CHECK(ap3.match("?")) ;
        CHECK_FALSE(ap3.match("x")) ;

        CHECK(ap4.match(".")) ;
        CHECK_FALSE(ap4.match("b")) ;

        CHECK(ap5.match("~")) ;
        CHECK_FALSE(ap5.match("n")) ;

        CHECK(ap6.match("c")) ;
        CHECK(ap6.match("h")) ;
        CHECK_FALSE(ap6.match("l")) ;
        CHECK_FALSE(ap6.match("3")) ;
    }

    SUBCASE("Complex") {
        AutoPattern ap1("*.exe") ;
        AutoPattern ap2("*/vim*") ;
        AutoPattern ap3("*/vim/entry.*") ;
        AutoPattern ap4("?:/*/win-vind/*/*.exe") ;
        AutoPattern ap5("*(vim|emacs)[0-9].[0-9]/*") ;
        AutoPattern ap6("msedge.exe") ;

        CHECK(ap1.match("C:/abc/defj/x.exe")) ;
        CHECK(ap1.match("C:/abc/defj/fjalkfa.exe")) ;
        CHECK_FALSE(ap1.match("C:/abc/defj/x.exea")) ;

        CHECK(ap2.match("C:/Program Files (x86)/fjaklf/vim/vim.exe")) ;
        CHECK(ap2.match("C:/Program Files (x86)/vim-like-something/gui.exe")) ;
        CHECK_FALSE(ap2.match("C:/Program Files (x86)/like-something/gui.exe")) ;

        CHECK(ap3.match("C:/Program Files (x86)/vim/entry.exe")) ;
        CHECK_FALSE(ap3.match("C:/Program Files (x86)/vim/vim.exe")) ;

        CHECK(ap4.match("C:/Program Files (x86)/win-vind/bin/win-vind.exe")) ;
        CHECK(ap4.match("C:/Program Files (x86)/win-vind/bin/gui.exe")) ;
        CHECK(ap4.match("D:/Program Files (x86)/win-vind/bin/win-vind.exe")) ;
        CHECK_FALSE(ap4.match("D:/win-vind/bin/win-vind.exe")) ;

        CHECK(ap5.match("D:/Program Files (x86)/vim8.1/bin/vim.exe")) ;
        CHECK(ap5.match("D:/Program Files (x86)/emacs1.0/bin/emacs.exe")) ;
        CHECK_FALSE(ap5.match("D:/Program Files (x86)/emacs1.2.0/bin/emacs.exe")) ;
        CHECK_FALSE(ap5.match("D:/Program Files (x86)/vscode1.2/bin/code.exe")) ;

        CHECK(ap6.match("D:/Program Files (x86)/microsoft/msedge.exe")) ;
        CHECK(ap6.match("msedge.exe")) ;
    }
}

TEST_CASE("AutoEvent") {
    AutoEvent evt ;
    auto cmd1 = core::parse_command("hjkl") ;
    evt.add_pattern_cmd("*notepad*", cmd1) ;

    auto cmd2 = core::parse_command("<shift-s><ctrl-b>") ;
    evt.add_pattern_cmd("*/vim*", cmd2) ;

    auto cmd3 = core::parse_command("abcd") ;
    evt.add_pattern_cmd("*/vim*", cmd3) ;

    auto cmd4 = core::parse_command("<shift-s><ctrl-b>jj") ;
    evt.add_pattern_cmd("*/vim-like/*", cmd4) ;

    auto cmd5 = core::parse_command("FF") ;
    evt.add_pattern_cmd("*edge*", cmd5) ;

    std::unordered_set<int> indices ;
    evt.match_pattern("C:/Program Files/vim-like/something.exe", indices) ;
    CHECK_EQ(indices.size(), 2) ;
    CHECK(indices.find(1) != indices.end()) ;
    CHECK(indices.find(2) != indices.end()) ;

    const auto& cmd_list_1 = evt.get_sequential_command(1) ;
    CHECK_EQ(cmd_list_1.size(), 2) ;
    CHECK(cmd_list_1[0] == cmd2) ;
    CHECK(cmd_list_1[1] == cmd3) ;

    const auto& cmd_list_2 = evt.get_sequential_command(2) ;
    CHECK_EQ(cmd_list_2.size(), 1) ;
    CHECK(cmd_list_2[0] == cmd4) ;

    indices.clear() ;

    evt.remove_pattern("*/vim*") ;
    evt.match_pattern("C:/Program Files/vim-like/something.exe", indices) ;
    CHECK_EQ(indices.size(), 1) ;

    const auto& cmd_list_3 = evt.get_sequential_command(1) ;
    CHECK_EQ(cmd_list_3.size(), 1) ;
    CHECK(cmd_list_3[0] == cmd4) ;
}
