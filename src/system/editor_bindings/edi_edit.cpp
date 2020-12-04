#include "edi_edit.hpp"

#include <iostream> //for debug
#include <memory>
#include <windows.h>

#include "edi_change_mode.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "simpl_text_selecter.hpp"
#include "smart_clipboard.hpp"
#include "system.hpp"
#include "text_analyzer.hpp"
#include "utility.hpp"

namespace ECBUtility
{
    enum class _RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;
    static auto _rgtype = _RegisteredType::None ;

    //Some editors have a visible EOL mark in a line.
    //This function select text from current position to EOL, except for the visible EOL mark.
    //If the line has only null characters, it does not select.
    //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
    //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
    inline bool _select_line_until_EOL(const TextAnalyzer::SelRes* const exres) {
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
                pushup(VKC_LCTRL, VKC_INSERT) ;
        }) ;
        if(res.having_EOL) {
            pushup(VKC_LSHIFT, VKC_LEFT) ;
            if(res.str.empty()) {
                return false ;
            }
        }
        return true ;
    }

    inline void _copy_null() {
        const auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            throw RUNTIME_EXCEPT("not exist active window") ;
        }
        SmartClipboard scb(hwnd) ;
        scb.open() ;
        scb.set("") ;
        scb.close() ;
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace ECBUtility ;
    using namespace ModeManager ;

    if(!first_call) {
        return ;
    }
    KeybrdEventer::pushup(VKC_LCTRL, VKC_INSERT) ;

    if(get_mode() == Mode::EdiLineVisual) {
        _rgtype = _RegisteredType::Lines ;
    }
    else {
        _rgtype = _RegisteredType::Chars ;
    }
    Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    SimplTextSelecter::unselect() ;
}


//EdiNCopyLine (EdiNormal only)
const std::string EdiNCopyLine::sname() noexcept
{
    return "edi_n_copy_line" ;
}
void EdiNCopyLine::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres)
{
    using KeybrdEventer::pushup ;
    if(!first_call) {
        return ;
    }

    pushup(VKC_HOME) ;

    for(unsigned int i = 1 ; i < repeat_num ; i ++) {
        pushup(VKC_LSHIFT, VKC_DOWN) ;
    }

    if(!ECBUtility::_select_line_until_EOL(exres)) {
        ECBUtility::_copy_null() ;
    }

    pushup(VKC_LCTRL, VKC_INSERT) ;
    pushup(VKC_END) ;

    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;
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

const std::string EdiNPasteAfter::sname() noexcept
{
    return "edi_n_paste_after" ;
}
void EdiNPasteAfter::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto paste = [repeat_num] {
        using ECBUtility::_RegisteredType ;
        using ECBUtility::_rgtype ;
        using KeybrdEventer::pushup ;

        if(_rgtype == _RegisteredType::Chars) {
            pushup(VKC_RIGHT) ;//move to after
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                pushup(VKC_LSHIFT, VKC_INSERT) ;
            }
        }
        else if(_rgtype == _RegisteredType::Lines) {
            pushup(VKC_END) ;
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                pushup(VKC_ENTER) ;
                pushup(VKC_LSHIFT, VKC_INSERT) ;
            }
        }
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        paste() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        paste() ;
    }
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto paste = [repeat_num] {
        using ECBUtility::_RegisteredType ;
        using ECBUtility::_rgtype ;
        using KeybrdEventer::pushup ;

        if(_rgtype == _RegisteredType::Chars) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                pushup(VKC_LSHIFT, VKC_INSERT) ;
            }
            Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
        }
        else if(_rgtype == _RegisteredType::Lines) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                pushup(VKC_HOME) ;
                pushup(VKC_ENTER) ;
                pushup(VKC_UP) ;
                pushup(VKC_LSHIFT, VKC_INSERT) ;
            }
        }
    } ;
    if(first_call) {
        pimpl->ksr.reset() ;
        paste() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        paste() ;
    }
}


//EdiDeleteHighlightText (Visual only)
const std::string EdiDeleteHighlightText::sname() noexcept
{
    return "edi_delete_highlight_text" ;
}
void EdiDeleteHighlightText::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace ECBUtility ;
    using namespace ModeManager ;

    if(!first_call) {
        return ;
    }

    KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;

    if(get_mode() == Mode::EdiLineVisual) {
        _rgtype = _RegisteredType::Lines ;
    }
    else {
        _rgtype = _RegisteredType::Chars ;
    }
    Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
}


namespace ECBUtility
{
    inline static void delete_line_when_selecting() {
        using namespace ModeManager ;
        const auto mode = get_mode() ;
        if(mode == Mode::EdiVisual) {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
            _rgtype = _RegisteredType::Chars ;
        }
        else if(mode == Mode::EdiLineVisual) {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            KeybrdEventer::pushup(VKC_DELETE) ;
            Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
            _rgtype = _RegisteredType::Lines ;
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres) const
{
    auto del = [exres, repeat_num] {
        //not select
        SimplTextSelecter::select_line_BOL2EOL() ;
        KeybrdEventer::pushup(VKC_HOME) ;
        for(unsigned int i = 1 ; i < repeat_num ; i ++) {
            KeybrdEventer::pushup(VKC_DOWN) ;
        }

        if(!ECBUtility::_select_line_until_EOL(exres)) {
            ECBUtility::_copy_null() ;
        }
        else {
            KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            KeybrdEventer::pushup(VKC_DELETE) ;
        }
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;

        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        del() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        del() ;
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres) const
{
    using namespace ECBUtility ;
    auto del = [exres] {
        //not select
        using KeybrdEventer::pushup ;
        if(!_select_line_until_EOL(exres)) {
            _copy_null() ;
        }
        else {
            pushup(VKC_LCTRL, VKC_X) ;
        }

        _rgtype = _RegisteredType::Chars ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        del() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        del() ;
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto del = [] {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;
    } ;
    if(first_call) {
        pimpl->ksr.reset() ;
        del() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        del() ;
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto del = [] {
        ECBUtility::delete_line_when_selecting() ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LEFT) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        del() ;
    }
    else if(pimpl->ksr.is_pressed()) {
        del() ;
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
        const KeyLogger* parent_vkclgr,
        const KeyLogger* const parent_charlgr)
{
    using Utility::remove_from_back ;
    if(!first_call) {
        return ;
    }

    KeyLogger lgr ;
    while(true) {
        Sleep(10) ;
        Utility::get_win_message() ;

        if(!KyLgr::log_as_vkc(lgr)) {
            remove_from_back(lgr, 1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(lgr,
                    KeyBinder::InvalidPolicy::UnbindedSystemKey)) {
            remove_from_back(lgr, 1) ;
            continue ;
        }

        if(auto func = KeyBinder::find_func(lgr)) {
            if(!func->is_for_moving_caret()) {
                return ;
            }

            if(func->is_callable()) {
                std::cout << func->name() << " is matched\n" ;
                func->process(true, 1, &lgr) ;
                break ;
            }
        }
        else {
            lgr.clear() ;
        }
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
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) {
        return ;
    }

    auto res = TextAnalyzer::get_selected_text([] {
        KeybrdEventer::pushup(VKC_HOME) ;
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_END) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_INSERT) ;
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

    for(int i = 0 ; i < static_cast<int>(pos) ; i ++) {
        KeybrdEventer::pushup(VKC_RIGHT) ;
    }
    EdiDeleteUntilEOLAndStartInsert::sprocess(true, 1, nullptr, nullptr, &res) ;
}


//EdiDeleteCharsAndStartInsert
const std::string EdiDeleteCharsAndStartInsert::sname() noexcept
{
    return "edi_delete_chars_and_start_insert" ;
}
void EdiDeleteCharsAndStartInsert::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) {
        return ;
    }
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
    }
    KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
}


//EdiDeleteUntilEOLAndStartInsert
const std::string EdiDeleteUntilEOLAndStartInsert::sname() noexcept
{
    return "edi_delete_until_eol_and_start_insert" ;
}
void EdiDeleteUntilEOLAndStartInsert::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr),
        const TextAnalyzer::SelRes* const exres)
{
    if(!first_call) {
        return ;
    }

    if(!ECBUtility::_select_line_until_EOL(exres)) {
        ECBUtility::_copy_null() ;
    }
    KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    Change2EdiInsert::sprocess(true, 1, nullptr, nullptr) ;
}
