#include "edi_edit.hpp"

#include <stdio.h>
#include <windows.h>

#include <memory>   //for std::unique_ptr
#include <iostream> //for debug

#include "edi_change_mode.hpp"
#include "i_params.hpp"
#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycodecvt.hpp"
#include "bind.hpp"
#include "mode.hpp"
#include "err_logger.hpp"
#include "text/simple_text_selecter.hpp"
#include "text/smart_clipboard.hpp"
#include "text/text_analyzer.hpp"
#include "time/keystroke_repeater.hpp"
#include "utility.hpp"
#include "entry.hpp"

namespace
{
    using namespace vind ;

    enum class RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;
    auto g_rgtype = RegisteredType::None ;

    //Some editors have a visible EOL mark in a line.
    //This function select text from current position to EOL except for the visible EOL mark.
    //If the line has only null characters, it does not select.
    //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
    //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
    inline bool select_line_until_EOL(const textanalyze::SelRes* const exres) {
        using namespace keybrd ;
        if(exres != nullptr) {
            pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            if(exres->having_EOL) {
                pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(exres->str.empty()) {
                    return false ; //not selected (true text is only null text)
                }
            }
            return true ; //selected
        }

        auto res = textanalyze::get_selected_text([] {
                pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;
        if(res.having_EOL) {
            pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
            if(res.str.empty()) {
                return false ;
            }
        }
        return true ;
    }

    inline void clear_clipboard_with_null() {
        const auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            throw RUNTIME_EXCEPT("not exist active window") ;
        }
        SmartClipboard scb(hwnd) ;
        scb.open() ;
        scb.set("") ;
        scb.close() ;
    }

    //return: is succecced?
    inline bool select_by_motion(unsigned int repeat_num, KeycodeLogger* const parent_vkclgr) {
        using namespace mode ;
        KeycodeLogger lgr ;

        keyabsorber::InstantKeyAbsorber ika ;

        while(vind::update_background()) {
            lgr.update() ;
            if(!lgr.is_changed()) {
                lgr.remove_from_back(1) ;
                continue ;
            }

            parent_vkclgr->update() ;

            if(keybind::is_invalid_log(lgr.latest(),
                        keybind::InvalidPolicy::UnbindedSystemKey)) {
                parent_vkclgr->remove_from_back(1) ;
                lgr.remove_from_back(1) ;
                continue ;
            }

            //The parent logger is stronger than the child logger.
            //For example, the child BindedFunc calling this function is binded with 'c{motion}'
            //and 'cc' are bindings EdiDeleteLinesAndStartInsert.
            //In this case, if a child process has a message loop, we must consider the parent logger by full scanning.
            if(auto func = keybind::find_func(*parent_vkclgr, nullptr, true)) {
                if(func->is_callable()) {
                    func->process(true, repeat_num, parent_vkclgr) ;
                    return false ;
                }
            }

            if(auto func = keybind::find_func(lgr, nullptr, true,
                        mode::Mode::EdiLineVisual)) {
                if(!func->is_for_moving_caret()) return false ;

                if(func->is_callable()) {
                    change_mode(Mode::EdiLineVisual) ;
                    func->process(true, repeat_num, &lgr) ;
                    return true ;
                }
            }
            else {
                return false ;
            }
        }

        return false ;
    }
}


namespace vind
{
    //EdiCopyHighlightText (EdiVisual only)
    const std::string EdiCopyHighlightText::sname() noexcept {
        return "edi_copy_highlight_text" ;
    }
    void EdiCopyHighlightText::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;

        if(!first_call) return ;

        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;

        if(get_mode() == Mode::EdiLineVisual)
            g_rgtype = RegisteredType::Lines ;
        else
            g_rgtype = RegisteredType::Chars ;

        keybrd::pushup(KEYCODE_LEFT) ; //unselect, but this is for the time being
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    }


    //EdiNCopyLine (EdiNormal only)
    const std::string EdiNCopyLine::sname() noexcept {
        return "edi_n_copy_line" ;
    }
    void EdiNCopyLine::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const textanalyze::SelRes* const exres) {
        if(!first_call) return ;

        using keybrd::pushup ;

        pushup(KEYCODE_HOME) ;

        //copy N - 1 lines
        for(unsigned int i = 1 ; i < repeat_num ; i ++)
            pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

        if(!select_line_until_EOL(exres))
            clear_clipboard_with_null() ;

        pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        pushup(KEYCODE_END) ;

        g_rgtype = RegisteredType::Lines ;
    }

    //EdiCopyMotion
    const std::string EdiCopyMotion::sname() noexcept {
        return "edi_copy_motion" ;
    }
    void EdiCopyMotion::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const parent_vkclgr,
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        if(select_by_motion(repeat_num, parent_vkclgr)) {
            EdiCopyHighlightText::sprocess(true, 1, nullptr, nullptr) ;
            Change2EdiNormal::sprocess(true, 1, nullptr, nullptr, false) ;
        }
    }
}


namespace
{

    template <typename T1,
              typename T2,
              typename T3,
              typename T4>
    inline void put_with_common_process(
            const bool first_call,
            unsigned int repeat_num,
            KeyStrokeRepeater& ksr,
            T1&& put_chars_preproc,
            T2&& put_chars,
            T3&& put_lines_preproc,
            T4&& put_lines) {

        if(repeat_num == 1) {
            if(first_call) {
                ksr.reset() ;
                if(g_rgtype == RegisteredType::Chars) {
                    put_chars_preproc() ;
                    put_chars() ;
                }
                else if(g_rgtype == RegisteredType::Lines) {
                    put_lines_preproc() ;
                    put_lines() ;
                }
            }
            else if(ksr.is_pressed()) {
                if(g_rgtype == RegisteredType::Chars) {
                    put_chars_preproc() ;
                    put_chars() ;
                }
                else if(g_rgtype == RegisteredType::Lines) {
                    put_lines_preproc() ;
                    put_lines() ;
                }
            }
            return ;
        }
        //repeat_num >= 2
        if(first_call) {
            if(g_rgtype == RegisteredType::Chars) {
                put_chars_preproc() ;
                for(unsigned int i = 0 ; i < repeat_num ; i ++) put_chars() ;
            }
            else if(g_rgtype == RegisteredType::Lines) {
                put_lines_preproc() ;
                for(unsigned int i = 0 ; i < repeat_num ; i ++) put_lines() ;
            }
            ksr.reset() ;
        }
    }
}

namespace vind
{
    //EdiNPasteAfter (EdiNormal or EdiVisual)
    struct EdiNPasteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteAfter::EdiNPasteAfter()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteAfter::~EdiNPasteAfter() noexcept                  = default ;
    EdiNPasteAfter::EdiNPasteAfter(EdiNPasteAfter&&)            = default ;
    EdiNPasteAfter& EdiNPasteAfter::operator=(EdiNPasteAfter&&) = default ;

    const std::string EdiNPasteAfter::sname() noexcept {
        return "edi_n_paste_after" ;
    }
    void EdiNPasteAfter::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        auto put_chars_preproc = [] {
            pushup(KEYCODE_RIGHT) ;
        } ;
        auto put_chars = [] {
            pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        } ;

        auto put_lines_preproc = [] {
            pushup(KEYCODE_END) ;
        } ;
        auto put_lines = [] {
            pushup(KEYCODE_ENTER) ;
            pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        } ;

        put_with_common_process(
                first_call, repeat_num, pimpl->ksr,
                put_chars_preproc, put_chars,
                put_lines_preproc, put_lines) ;
    }

    //EdiNPasteBefore
    struct EdiNPasteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNPasteBefore::EdiNPasteBefore()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNPasteBefore::~EdiNPasteBefore() noexcept                   = default ;
    EdiNPasteBefore::EdiNPasteBefore(EdiNPasteBefore&&)            = default ;
    EdiNPasteBefore& EdiNPasteBefore::operator=(EdiNPasteBefore&&) = default ;

    const std::string EdiNPasteBefore::sname() noexcept {
        return "edi_n_paste_before" ;
    }
    void EdiNPasteBefore::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        using keybrd::pushup ;
        auto put_chars = [] {
            pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        } ;
        auto put_lines = [] {
            pushup(KEYCODE_HOME) ;
            pushup(KEYCODE_ENTER) ;
            pushup(KEYCODE_UP) ;
            pushup(KEYCODE_LCTRL, KEYCODE_V) ;
        } ;
        put_with_common_process(
                first_call, repeat_num, pimpl->ksr,
                []{return;}, put_chars,
                []{return;}, put_lines) ;
    }


    //EdiDeleteHighlightText (Visual only)
    const std::string EdiDeleteHighlightText::sname() noexcept {
        return "edi_delete_highlight_text" ;
    }
    void EdiDeleteHighlightText::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        using namespace mode ;
        using keybrd::pushup ;

        if(!first_call) return ;

        pushup(KEYCODE_LCTRL, KEYCODE_X) ;
        if(get_mode() == Mode::EdiLineVisual) {
            g_rgtype = RegisteredType::Lines ;
        }
        else {
            g_rgtype = RegisteredType::Chars ;
        }
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    }
}


namespace
{
    inline void delete_line_when_selecting() {
        using namespace mode ;
        const auto mode = get_mode() ;
        if(mode == Mode::EdiVisual) {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
            else {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
        }
        else if(mode == Mode::EdiLineVisual) {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            }
            else {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
            keybrd::pushup(KEYCODE_DELETE) ;
            g_rgtype = RegisteredType::Lines ;
        }
    }
}

namespace vind
{
    //EdiNDeleteLine
    struct EdiNDeleteLine::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLine::EdiNDeleteLine()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLine::~EdiNDeleteLine() noexcept                  = default ;
    EdiNDeleteLine::EdiNDeleteLine(EdiNDeleteLine&&)            = default ;
    EdiNDeleteLine& EdiNDeleteLine::operator=(EdiNDeleteLine&&) = default ;

    const std::string EdiNDeleteLine::sname() noexcept {
        return "edi_n_delete_line" ;
    }
    void EdiNDeleteLine::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const textanalyze::SelRes* const exres) const {
        using keybrd::pushup ;
        auto to_head = [] {pushup(KEYCODE_HOME) ;} ;
        auto del = [exres] {
            if(!select_line_until_EOL(exres)) {
                clear_clipboard_with_null() ;
            }
            else {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Lines ;
                pushup(KEYCODE_DELETE) ;
            }
        } ;

        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                to_head() ;
                del() ;
            }
            else if(pimpl->ksr.is_pressed()) {
                to_head() ;
                del() ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            to_head() ;

            for(unsigned int i = 1 ; i < repeat_num ; i ++)
                pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

            del() ;
            pimpl->ksr.reset() ;
        }
    }


    //EdiNDeleteLineUntilEOL
    struct EdiNDeleteLineUntilEOL::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteLineUntilEOL::~EdiNDeleteLineUntilEOL() noexcept                          = default ;
    EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&)            = default ;
    EdiNDeleteLineUntilEOL& EdiNDeleteLineUntilEOL::operator=(EdiNDeleteLineUntilEOL&&) = default ;

    const std::string EdiNDeleteLineUntilEOL::sname() noexcept {
        return "edi_n_delete_line_until_EOL" ;
    }
    void EdiNDeleteLineUntilEOL::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const textanalyze::SelRes* const exres) const {
        using keybrd::pushup ;

        auto del = [exres] {
            if(!select_line_until_EOL(exres)) {
                clear_clipboard_with_null() ;
            }
            else {
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
        } ;

        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                del() ;
            }
            else if(pimpl->ksr.is_pressed()) {
                del() ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            //delete N - 1 lines under the current line
            for(unsigned int i = 1 ; i < repeat_num ; i ++)
                pushup(KEYCODE_DOWN) ;

            del() ;
            pimpl->ksr.reset() ;
        }
    }

    //EdiNDeleteAfter
    struct EdiNDeleteAfter::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteAfter::EdiNDeleteAfter()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteAfter::~EdiNDeleteAfter() noexcept                   = default ;
    EdiNDeleteAfter::EdiNDeleteAfter(EdiNDeleteAfter&&)            = default ;
    EdiNDeleteAfter& EdiNDeleteAfter::operator=(EdiNDeleteAfter&&) = default ;

    const std::string EdiNDeleteAfter::sname() noexcept {
        return "edi_n_delete_after" ;
    }
    void EdiNDeleteAfter::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto del = [] {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
            else {
                keybrd::pushup(KEYCODE_DELETE) ;
            }
        } ;
        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                del() ;
            }
            else if(pimpl->ksr.is_pressed()) {
                del() ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                del() ;

            pimpl->ksr.reset() ;
        }
    }


    //EdiNDeleteBefore
    struct EdiNDeleteBefore::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    EdiNDeleteBefore::EdiNDeleteBefore()
    : pimpl(std::make_unique<Impl>())
    {}

    EdiNDeleteBefore::~EdiNDeleteBefore() noexcept                    = default ;
    EdiNDeleteBefore::EdiNDeleteBefore(EdiNDeleteBefore&&)            = default ;
    EdiNDeleteBefore& EdiNDeleteBefore::operator=(EdiNDeleteBefore&&) = default ;

    const std::string EdiNDeleteBefore::sname() noexcept {
        return "edi_n_delete_before" ;
    }
    void EdiNDeleteBefore::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto del = [] {
            if(iparams::get_b("enable_char_cache")) {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
                g_rgtype = RegisteredType::Chars ;
            }
            else {
                keybrd::pushup(KEYCODE_BKSPACE) ;
            }
        } ;

        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                del() ;
            }
            else if(pimpl->ksr.is_pressed()) {
                del() ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                del() ;

            pimpl->ksr.reset() ;
        }
    }


    //EdiDeleteMotion
    const std::string EdiDeleteMotion::sname() noexcept {
        return "edi_delete_motion" ;
    }
    void EdiDeleteMotion::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const parent_vkclgr,
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        if(select_by_motion(repeat_num, parent_vkclgr)) {
            EdiDeleteHighlightText::sprocess(true, 1, nullptr, nullptr) ;
        }
    }


    //EdiDeleteMotionAndStartInsert
    const std::string EdiDeleteMotionAndStartInsert::sname() noexcept {
        return "edi_delete_motion_and_start_insert" ;
    }
    void EdiDeleteMotionAndStartInsert::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const parent_vkclgr,
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        if(select_by_motion(repeat_num, parent_vkclgr)) {
            EdiDeleteHighlightText::sprocess(true, 1, nullptr, nullptr) ;
            Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
        }
    }


    //EdiDeleteLinesAndStartInsert
    const std::string EdiDeleteLinesAndStartInsert::sname() noexcept {
        return "edi_delete_lines_and_start_insert" ;
    }

    void EdiDeleteLinesAndStartInsert::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto res = textanalyze::get_selected_text([] {
            keybrd::pushup(KEYCODE_HOME) ;
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        }) ;
        if(res.str.empty()) {
            Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
            return ;
        }

        const auto pos = res.str.find_first_not_of(" \t") ; //position except for space or tab
        if(pos == std::string::npos) { //space only
            Change2EdiEOLInsert::sprocess(true, 1, nullptr, nullptr) ;
            return ;
        }
        keybrd::pushup(KEYCODE_HOME) ;

        for(int i = 0 ; i < static_cast<int>(pos) ; i ++)
            keybrd::pushup(KEYCODE_RIGHT) ;

        EdiDeleteUntilEOLAndStartInsert::sprocess(true, repeat_num, nullptr, nullptr, &res) ;
    }


    //EdiDeleteCharsAndStartInsert
    const std::string EdiDeleteCharsAndStartInsert::sname() noexcept {
        return "edi_delete_chars_and_start_insert" ;
    }
    void EdiDeleteCharsAndStartInsert::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;

        if(iparams::get_b("enable_char_cache")) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Chars ;
        }
        else {
            keybrd::pushup(KEYCODE_DELETE) ;
        }

        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
    }


    //EdiDeleteUntilEOLAndStartInsert
    const std::string EdiDeleteUntilEOLAndStartInsert::sname() noexcept {
        return "edi_delete_until_eol_and_start_insert" ;
    }
    /* Actually, If N >= 2
     *
     * Command: 2C
     * 
     * If the caret is positioned the head of a line.
     *
     * Original Vim:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *      |
     *          CCCCC
     *
     * win-vind:
     * [Before]
     *      |   AAAAAA
     *          BBBBBB
     *          CCCCCC
     * [After]
     *         |CCCCCC
     *
     * In future, must fix.
     *
     */
    void EdiDeleteUntilEOLAndStartInsert::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr),
            const textanalyze::SelRes* const exres) {
        if(!first_call) return ;

        for(unsigned int i = 1 ; i < repeat_num ; i ++)
            keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_DOWN) ;

        if(!select_line_until_EOL(exres)) {
            clear_clipboard_with_null() ;
        }
        else {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            g_rgtype = RegisteredType::Chars ;
        }
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
    }
}
