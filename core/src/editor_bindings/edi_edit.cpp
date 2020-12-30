#include "edi_edit.hpp"

#include <stdio.h>
#include <windows.h>

#include <memory>   //for std::unique_ptr
#include <iostream> //for debug

#include "edi_change_mode.hpp"
#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "simple_text_selecter.hpp"
#include "smart_clipboard.hpp"
#include "system.hpp"
#include "text_analyzer.hpp"
#include "utility.hpp"
#include "vkc_converter.hpp"

namespace EdiEdit
{
    enum class _RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;
    static auto g_rgtype = _RegisteredType::None ;

    //Some editors have a visible EOL mark in a line.
    //This function select text from current position to EOL except for the visible EOL mark.
    //If the line has only null characters, it does not select.
    //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
    //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
    inline static bool select_line_until_EOL(const TextAnalyzer::SelRes* const exres) {
        using namespace KeybrdEventer ;
        if(exres != nullptr) {
            pushup(VKC_LSHIFT, VKC_END) ;
            if(exres->having_EOL) {
                pushup(VKC_LSHIFT, VKC_LEFT) ;
                if(exres->str.empty()) {
                    return false ; //not selected (true text is only null text)
                }
            }
            return true ; //selected
        }

        auto res = TextAnalyzer::get_selected_text([] {
                pushup(VKC_LSHIFT, VKC_END) ;
                pushup(VKC_LCTRL, VKC_C) ;
        }) ;
        if(res.having_EOL) {
            pushup(VKC_LSHIFT, VKC_LEFT) ;
            if(res.str.empty()) {
                return false ;
            }
        }
        return true ;
    }

    inline static void _copy_null() {
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
    inline static bool select_by_motion(unsigned int repeat_num, KeyLogger* parent_vkclgr) {
        using Utility::remove_from_back ;
        using namespace ModeManager ;
        KeyLogger lgr ;

        for(auto key : KeyAbsorber::get_pressed_list())
            KeyAbsorber::release_vertually(key) ;

        while(true) {
            Sleep(30) ;
            Utility::get_win_message() ;

            if(!KyLgr::log_as_vkc(lgr)) {
                remove_from_back(lgr, 1) ;
                continue ;
            }

            KyLgr::log_as_vkc(*parent_vkclgr) ;

            if(KeyBinder::is_invalid_log(lgr.back(),
                        KeyBinder::InvalidPolicy::UnbindedSystemKey)) {
                remove_from_back(*parent_vkclgr, 1) ;
                remove_from_back(lgr, 1) ;
                continue ;
            }

            //The parent logger is stronger than the child logger.
            //For example, the child BindedFunc calling this function is binded with 'c{motion}'
            //and 'cc' are bindings EdiDeleteLinesAndStartInsert.
            //In this case, if a child process has a message loop, we must consider the parent logger by full scanning.
            if(auto func = KeyBinder::find_func(*parent_vkclgr, nullptr, true)) {
                if(func->is_callable()) {
                    func->process(true, repeat_num, parent_vkclgr) ;
                    return false ;
                }
            }

            if(auto func = KeyBinder::find_func(lgr, nullptr, true,
                        ModeManager::Mode::EdiLineVisual)) {
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
    }
}


//EdiCopyHighlightText (EdiVisual only)
const std::string EdiCopyHighlightText::sname() noexcept
{
    return "edi_copy_highlight_text" ;
}
void EdiCopyHighlightText::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace EdiEdit ;
    using namespace ModeManager ;

    if(!first_call) return ;

    KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;

    if(get_mode() == Mode::EdiLineVisual)
        g_rgtype = _RegisteredType::Lines ;
    else
        g_rgtype = _RegisteredType::Chars ;

    KeybrdEventer::pushup(VKC_LEFT) ; //unselect, but this is for the time being
    Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
}


//EdiNCopyLine (EdiNormal only)
const std::string EdiNCopyLine::sname() noexcept
{
    return "edi_n_copy_line" ;
}
void EdiNCopyLine::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres)
{
    if(!first_call) return ;

    using KeybrdEventer::pushup ;

    pushup(VKC_HOME) ;

    //copy N - 1 lines
    for(unsigned int i = 1 ; i < repeat_num ; i ++)
        pushup(VKC_LSHIFT, VKC_DOWN) ;

    if(!EdiEdit::select_line_until_EOL(exres))
        EdiEdit::_copy_null() ;

    pushup(VKC_LCTRL, VKC_C) ;
    pushup(VKC_END) ;

    EdiEdit::g_rgtype = EdiEdit::_RegisteredType::Lines ;
}

//EdiCopyMotion
const std::string EdiCopyMotion::sname() noexcept
{
    return "edi_copy_motion" ;
}
void EdiCopyMotion::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(EdiEdit::select_by_motion(repeat_num, parent_vkclgr)) {
        EdiCopyHighlightText::sprocess(true, 1, nullptr, nullptr) ;
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr, false) ;
    }
}

//EdiNPasteAfter (EdiNormal or EdiVisual)
struct EdiNPasteAfter::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNPasteAfter::EdiNPasteAfter()
: pimpl(std::make_unique<Impl>())
{}

EdiNPasteAfter::~EdiNPasteAfter() noexcept                  = default ;
EdiNPasteAfter::EdiNPasteAfter(EdiNPasteAfter&&)            = default ;
EdiNPasteAfter& EdiNPasteAfter::operator=(EdiNPasteAfter&&) = default ;


namespace EdiEdit
{

    template <typename T1,
              typename T2,
              typename T3,
              typename T4>
    inline static void _common_put_proc(
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
                if(g_rgtype == _RegisteredType::Chars) {
                    put_chars_preproc() ;
                    put_chars() ;
                }
                else if(g_rgtype == _RegisteredType::Lines) {
                    put_lines_preproc() ;
                    put_lines() ;
                }
            }
            else if(ksr.is_pressed()) {
                if(g_rgtype == _RegisteredType::Chars) {
                    put_chars_preproc() ;
                    put_chars() ;
                }
                else if(g_rgtype == _RegisteredType::Lines) {
                    put_lines_preproc() ;
                    put_lines() ;
                }
            }
            return ;
        }
        //repeat_num >= 2
        if(first_call) {
            if(g_rgtype == _RegisteredType::Chars) {
                put_chars_preproc() ;
                for(unsigned int i = 0 ; i < repeat_num ; i ++) put_chars() ;
            }
            else if(g_rgtype == _RegisteredType::Lines) {
                put_lines_preproc() ;
                for(unsigned int i = 0 ; i < repeat_num ; i ++) put_lines() ;
            }
            ksr.reset() ;
        }
    }
}

const std::string EdiNPasteAfter::sname() noexcept
{
    return "edi_n_paste_after" ;
}
void EdiNPasteAfter::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    auto put_chars_preproc = [] {
        pushup(VKC_RIGHT) ;
    } ;
    auto put_chars = [] {
        pushup(VKC_LCTRL, VKC_V) ;
    } ;

    auto put_lines_preproc = [] {
        pushup(VKC_END) ;
    } ;
    auto put_lines = [] {
        pushup(VKC_ENTER) ;
        pushup(VKC_LCTRL, VKC_V) ;
    } ;

    EdiEdit::_common_put_proc(
            first_call, repeat_num, pimpl->ksr,
            put_chars_preproc, put_chars,
            put_lines_preproc, put_lines) ;
}


//EdiNPasteBefore
struct EdiNPasteBefore::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNPasteBefore::EdiNPasteBefore()
: pimpl(std::make_unique<Impl>())
{}

EdiNPasteBefore::~EdiNPasteBefore() noexcept                   = default ;
EdiNPasteBefore::EdiNPasteBefore(EdiNPasteBefore&&)            = default ;
EdiNPasteBefore& EdiNPasteBefore::operator=(EdiNPasteBefore&&) = default ;

const std::string EdiNPasteBefore::sname() noexcept
{
    return "edi_n_paste_before" ;
}
void EdiNPasteBefore::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    auto put_chars = [] {
        pushup(VKC_LCTRL, VKC_V) ;
    } ;
    auto put_lines = [] {
        pushup(VKC_HOME) ;
        pushup(VKC_ENTER) ;
        pushup(VKC_UP) ;
        pushup(VKC_LCTRL, VKC_V) ;
    } ;
    EdiEdit::_common_put_proc(
            first_call, repeat_num, pimpl->ksr,
            []{return;}, put_chars,
            []{return;}, put_lines) ;
}


//EdiDeleteHighlightText (Visual only)
const std::string EdiDeleteHighlightText::sname() noexcept
{
    return "edi_delete_highlight_text" ;
}
void EdiDeleteHighlightText::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace EdiEdit ;
    using namespace ModeManager ;
    using KeybrdEventer::pushup ;

    if(!first_call) return ;

    pushup(VKC_LCTRL, VKC_X) ;
    if(get_mode() == Mode::EdiLineVisual) {
        g_rgtype = _RegisteredType::Lines ;
    }
    else {
        g_rgtype = _RegisteredType::Chars ;
    }
    Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
}


namespace EdiEdit
{
    inline static void delete_line_when_selecting() {
        using namespace ModeManager ;
        const auto mode = get_mode() ;
        if(mode == Mode::EdiVisual) {
            if(iParams::get_b("enable_char_cache")) {
                KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
                g_rgtype = _RegisteredType::Chars ;
            }
            else {
                KeybrdEventer::pushup(VKC_DELETE) ;
            }
        }
        else if(mode == Mode::EdiLineVisual) {
            if(iParams::get_b("enable_char_cache")) {
                KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            }
            else {
                KeybrdEventer::pushup(VKC_DELETE) ;
            }
            KeybrdEventer::pushup(VKC_DELETE) ;
            g_rgtype = _RegisteredType::Lines ;
        }
    }
}

//EdiNDeleteLine
struct EdiNDeleteLine::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteLine::EdiNDeleteLine()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteLine::~EdiNDeleteLine() noexcept                  = default ;
EdiNDeleteLine::EdiNDeleteLine(EdiNDeleteLine&&)            = default ;
EdiNDeleteLine& EdiNDeleteLine::operator=(EdiNDeleteLine&&) = default ;

const std::string EdiNDeleteLine::sname() noexcept
{
    return "edi_n_delete_line" ;
}
void EdiNDeleteLine::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres) const
{
    using KeybrdEventer::pushup ;
    auto to_head = [] {pushup(VKC_HOME) ;} ;
    auto del = [exres] {
        if(!EdiEdit::select_line_until_EOL(exres)) {
            EdiEdit::_copy_null() ;
        }
        else {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            EdiEdit::g_rgtype = EdiEdit::_RegisteredType::Lines ;
            pushup(VKC_DELETE) ;
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
            pushup(VKC_LSHIFT, VKC_DOWN) ;

        del() ;
        pimpl->ksr.reset() ;
    }
}


//EdiNDeleteLineUntilEOL
struct EdiNDeleteLineUntilEOL::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteLineUntilEOL::~EdiNDeleteLineUntilEOL() noexcept                          = default ;
EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&)            = default ;
EdiNDeleteLineUntilEOL& EdiNDeleteLineUntilEOL::operator=(EdiNDeleteLineUntilEOL&&) = default ;

const std::string EdiNDeleteLineUntilEOL::sname() noexcept
{
    return "edi_n_delete_line_until_EOL" ;
}
void EdiNDeleteLineUntilEOL::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres) const
{
    using namespace EdiEdit ;
    using KeybrdEventer::pushup ;

    auto del = [exres] {
        if(!select_line_until_EOL(exres)) {
            _copy_null() ;
        }
        else {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            g_rgtype = _RegisteredType::Chars ;
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
            pushup(VKC_DOWN) ;

        del() ;
        pimpl->ksr.reset() ;
    }
}

//EdiNDeleteAfter
struct EdiNDeleteAfter::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteAfter::EdiNDeleteAfter()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteAfter::~EdiNDeleteAfter() noexcept                   = default ;
EdiNDeleteAfter::EdiNDeleteAfter(EdiNDeleteAfter&&)            = default ;
EdiNDeleteAfter& EdiNDeleteAfter::operator=(EdiNDeleteAfter&&) = default ;

const std::string EdiNDeleteAfter::sname() noexcept
{
    return "edi_n_delete_after" ;
}
void EdiNDeleteAfter::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto del = [] {
        if(iParams::get_b("enable_char_cache")) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            EdiEdit::g_rgtype = EdiEdit::_RegisteredType::Chars ;
        }
        else {
            KeybrdEventer::pushup(VKC_DELETE) ;
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
struct EdiNDeleteBefore::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteBefore::EdiNDeleteBefore()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteBefore::~EdiNDeleteBefore() noexcept                    = default ;
EdiNDeleteBefore::EdiNDeleteBefore(EdiNDeleteBefore&&)            = default ;
EdiNDeleteBefore& EdiNDeleteBefore::operator=(EdiNDeleteBefore&&) = default ;

const std::string EdiNDeleteBefore::sname() noexcept
{
    return "edi_n_delete_before" ;
}
void EdiNDeleteBefore::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto del = [] {
        if(iParams::get_b("enable_char_cache")) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_LEFT) ;
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            EdiEdit::g_rgtype = EdiEdit::_RegisteredType::Chars ;
        }
        else {
            KeybrdEventer::pushup(VKC_BKSPACE) ;
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
const std::string EdiDeleteMotion::sname() noexcept
{
    return "edi_delete_motion" ;
}
void EdiDeleteMotion::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(EdiEdit::select_by_motion(repeat_num, parent_vkclgr)) {
        EdiDeleteHighlightText::sprocess(true, 1, nullptr, nullptr) ;
    }
}


//EdiDeleteMotionAndStartInsert
const std::string EdiDeleteMotionAndStartInsert::sname() noexcept
{
    return "edi_delete_motion_and_start_insert" ;
}
void EdiDeleteMotionAndStartInsert::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* parent_vkclgr,
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(EdiEdit::select_by_motion(repeat_num, parent_vkclgr)) {
        EdiDeleteHighlightText::sprocess(true, 1, nullptr, nullptr) ;
        Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
    }
}


//EdiDeleteLinesAndStartInsert
const std::string EdiDeleteLinesAndStartInsert::sname() noexcept
{
    return "edi_delete_lines_and_start_insert" ;
}

void EdiDeleteLinesAndStartInsert::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto res = TextAnalyzer::get_selected_text([] {
        KeybrdEventer::pushup(VKC_HOME) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;
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
    KeybrdEventer::pushup(VKC_HOME) ;

    for(int i = 0 ; i < static_cast<int>(pos) ; i ++)
        KeybrdEventer::pushup(VKC_RIGHT) ;

    EdiDeleteUntilEOLAndStartInsert::sprocess(true, repeat_num, nullptr, nullptr, &res) ;
}


//EdiDeleteCharsAndStartInsert
const std::string EdiDeleteCharsAndStartInsert::sname() noexcept
{
    return "edi_delete_chars_and_start_insert" ;
}
void EdiDeleteCharsAndStartInsert::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    for(unsigned int i = 0 ; i < repeat_num ; i ++)
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;

    if(iParams::get_b("enable_char_cache")) {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
        EdiEdit::g_rgtype = EdiEdit::_RegisteredType::Chars ;
    }
    else {
        KeybrdEventer::pushup(VKC_DELETE) ;
    }

    Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
}


//EdiDeleteUntilEOLAndStartInsert
const std::string EdiDeleteUntilEOLAndStartInsert::sname() noexcept
{
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres)
{
    using namespace EdiEdit ;

    if(!first_call) return ;

    for(unsigned int i = 1 ; i < repeat_num ; i ++)
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_DOWN) ;

    if(!select_line_until_EOL(exres)) {
        _copy_null() ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
        g_rgtype = _RegisteredType::Chars ;
    }
    Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
}
