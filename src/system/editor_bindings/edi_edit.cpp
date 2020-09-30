#include "edi_edit.hpp"

#include <iostream> //for debug
#include <memory>
#include <windows.h>

#include "edi_change_mode.hpp"
#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "simpl_text_selecter.hpp"
#include "text_analyzer.hpp"
#include "utility.hpp"
#include "smart_clipboard.hpp"
#include "system.hpp"
#include "key_logger.hpp"
#include "key_binder.hpp"

namespace ECBUtility
{
    enum class _RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;

    static _RegisteredType _rgtype{_RegisteredType::None} ;

    //Some editors have a visible EOL mark in a line.
    //This function select text from current position to EOL, except for the visible EOL mark.
    //If the line has only null characters, it does not select.
    //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
    //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
    inline bool select_line_until_EOL(const TextAnalyzer::SelRes* const exres) noexcept {
        using namespace KeybrdEventer ;
        if(exres != nullptr) {
            if(!pushup(VKC_LSHIFT, VKC_END)) {
                return false ;
            }
            if(exres->having_EOL) {
                if(!pushup(VKC_LSHIFT, VKC_LEFT)) {
                    return false ;
                }
                if(exres->str.empty()) {
                    return false ; //not selected (true text is only null text)
                }
            }
            return true ; //selected
        }

        if(auto res = TextAnalyzer::get_selected_text([] {
                if(!pushup(VKC_LSHIFT, VKC_END)) {
                    return false ;
                }
                if(!pushup(VKC_LCTRL, VKC_INSERT)) {
                    return false ;
                }
                return true ;
            })) {

            if(res.having_EOL) {
                if(!pushup(VKC_LSHIFT, VKC_LEFT)) {
                    return false ;
                }
                if(res.str.empty()) {
                    return false ;
                }
            }
            return true ;
        }
        return false ;
    }

    inline bool copy_null() noexcept {
        const auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            WIN_ERROR_PRINT("not exist active window") ;
            return false ;
        }

        SmartClipboard scb(hwnd) ;
        if(!scb.open()) {
            return false ;
        }
        if(!scb.set("")) {
            return false ;
        }

        if(!scb.close()) {
            return false ;
        }

        return true ;
    }
}


//EdiCopyHighlightText (EdiVisual only)
const std::string EdiCopyHighlightText::sname() noexcept
{
    return "edi_copy_highlight_text" ;
}
bool EdiCopyHighlightText::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }

    using namespace ECBUtility ;
    using namespace ModeManager ;
    if(get_mode() == Mode::EdiLineVisual) {
        _rgtype = _RegisteredType::Lines ;
    }
    else {
        _rgtype = _RegisteredType::Chars ;
    }

    if(!Change2EdiNormal::sprocess(true)) {
        return false ;
    }
    if(!SimplTextSelecter::unselect()) {
        return false ;
    }
    return true ;
}


//EdiNCopyLine (EdiNormal only)
const std::string EdiNCopyLine::sname() noexcept
{
    return "edi_n_copy_line" ;
}
bool EdiNCopyLine::sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres)
{
    if(!first_call) {
        return true ;
    }
    using KeybrdEventer::pushup ;
    if(!pushup(VKC_HOME)) {
        return false ;
    }
    if(!ECBUtility::select_line_until_EOL(exres)) {
        if(!ECBUtility::copy_null()) {
            return false ;
        }
    }

    if(!pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }
    if(!pushup(VKC_END)) {
        return false ;
    }

    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;
    return true ;
}

//EdiNPasteAfter (EdiNormal or EdiVisual)
struct EdiNPasteAfter::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNPasteAfter::EdiNPasteAfter()
: pimpl(std::make_unique<Impl>())
{}

EdiNPasteAfter::~EdiNPasteAfter() noexcept = default ;
EdiNPasteAfter::EdiNPasteAfter(EdiNPasteAfter&&) noexcept = default ;
EdiNPasteAfter& EdiNPasteAfter::operator=(EdiNPasteAfter&&) noexcept = default ;

const std::string EdiNPasteAfter::sname() noexcept
{
    return "edi_n_paste_after" ;
}
bool EdiNPasteAfter::sprocess(const bool first_call) const
{
    auto paste = [] {
        using ECBUtility::_RegisteredType ;
        using ECBUtility::_rgtype ;
        using KeybrdEventer::pushup ;

        if(_rgtype == _RegisteredType::Chars) {
            if(!pushup(VKC_LSHIFT, VKC_INSERT)) {
                return false ;
            }

            if(!Change2EdiNormal::sprocess(true)) {
                return false ;
            }
            return true ;
        }

        if(_rgtype == _RegisteredType::Lines) {
            if(ModeManager::is_edi_visual()) {
                return true ;
            }
            if(!pushup(VKC_END)) {
                return false ;
            }
            //It might be double newlines in some applications distinglishing EOL.
            //In other words, creating a empty line.
            if(!pushup(VKC_ENTER)) {
                return false ;
            }
            if(!pushup(VKC_LSHIFT, VKC_INSERT)) {
                return false ;
            }

            return true ;
        }
        return true ;
    } ;
    if(first_call) {
        pimpl->ksr.reset() ;
        return paste() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return paste() ;
    }
    return true ;
}


//EdiNPasteBefore
struct EdiNPasteBefore::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNPasteBefore::EdiNPasteBefore()
: pimpl(std::make_unique<Impl>())
{}

EdiNPasteBefore::~EdiNPasteBefore() noexcept = default ;
EdiNPasteBefore::EdiNPasteBefore(EdiNPasteBefore&&) noexcept = default ;
EdiNPasteBefore& EdiNPasteBefore::operator=(EdiNPasteBefore&&) noexcept = default ;
const std::string EdiNPasteBefore::sname() noexcept
{
    return "edi_n_paste_before" ;
}
bool EdiNPasteBefore::sprocess(const bool first_call) const
{
    auto paste = [] {
        using ECBUtility::_RegisteredType ;
        using ECBUtility::_rgtype ;
        using KeybrdEventer::pushup ;

        if(_rgtype == _RegisteredType::Chars) {
            if(!pushup(VKC_LSHIFT, VKC_INSERT)) {
                return false ;
            }
            if(!Change2EdiNormal::sprocess(true)) {
                return false ;
            }
            return true ;
        }

        if(_rgtype == _RegisteredType::Lines) {
            if(ModeManager::is_edi_visual()) {
                return true ;
            }
            if(!pushup(VKC_HOME)) {
                return false ;
            }
            if(!pushup(VKC_ENTER)) {
                return false ;
            }
            if(!pushup(VKC_UP)) {
                return false ;
            }
            if(!pushup(VKC_LSHIFT, VKC_INSERT)) {
                return false ;
            }
            return true ;
        }
        return true ;
    } ;
    if(first_call) {
        pimpl->ksr.reset() ;
        return paste() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return paste() ;
    }
    return true ;
}


//EdiDeleteHighlightText (Visual only)
const std::string EdiDeleteHighlightText::sname() noexcept
{
    return "edi_delete_highlight_text" ;
}
bool EdiDeleteHighlightText::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }

    using namespace ECBUtility ;
    using namespace ModeManager ;
    if(get_mode() == Mode::EdiLineVisual) {
        _rgtype = _RegisteredType::Lines ;
    }
    else {
        _rgtype = _RegisteredType::Chars ;
    }

    if(!Change2EdiNormal::sprocess(true)) {
        return false ;
    }
    return true ;
}


namespace ECBUtility
{

    inline static bool delete_line() {
        using KeybrdEventer::pushup ;
        if(!pushup(VKC_LCTRL, VKC_X)) {
            return false ;
        }
        if(!pushup(VKC_DELETE)) {
            return false ;
        }
        /*
        if(!pushup(VKC_BKSPACE)) {
            return false ;
        }
        if(!pushup(VKC_LCTRL, VKC_RIGHT)) {
            return false ;
        }
        */

        return true ;
    }

    inline static bool delete_line_when_selecting() {
        using namespace ModeManager ;
        const auto mode = get_mode() ;
        if(mode == Mode::EdiVisual) {
            if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_X)) {
                return false ;
            }

            if(!Change2EdiNormal::sprocess(true)) {
                return false ;
            }
            _rgtype = _RegisteredType::Chars ;
            return true ;
        }
        if(mode == Mode::EdiLineVisual) {
            if(!ECBUtility::delete_line()) {
                return false ;
            }

            if(!Change2EdiNormal::sprocess(true)) {
                return false ;
            }
            _rgtype = _RegisteredType::Lines ;
            return true ;
        }

        return false ;
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

EdiNDeleteLine::~EdiNDeleteLine() noexcept = default ;
EdiNDeleteLine::EdiNDeleteLine(EdiNDeleteLine&&) noexcept = default ;
EdiNDeleteLine& EdiNDeleteLine::operator=(EdiNDeleteLine&&) noexcept = default ;
const std::string EdiNDeleteLine::sname() noexcept
{
    return "edi_n_delete_line" ;
}
bool EdiNDeleteLine::sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres) const
{
    auto del = [exres] {
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        if(!SimplTextSelecter::select_line_BOL2EOL()) {
            return false ;
        }

        if(!KeybrdEventer::pushup(VKC_HOME)) {
            return false ;
        }
        if(!ECBUtility::select_line_until_EOL(exres)) {
            if(!ECBUtility::copy_null()) {
                return false ;
            }
        }

        if(!ECBUtility::delete_line()) {
            return false ;
        }
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;
        return true ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return del() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return del() ;
    }
    return true ;
}


//EdiNDeleteLineUntilEOL
struct EdiNDeleteLineUntilEOL::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteLineUntilEOL::~EdiNDeleteLineUntilEOL() noexcept = default ;
EdiNDeleteLineUntilEOL::EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&) noexcept = default ;
EdiNDeleteLineUntilEOL& EdiNDeleteLineUntilEOL::operator=(EdiNDeleteLineUntilEOL&&) noexcept = default ;
const std::string EdiNDeleteLineUntilEOL::sname() noexcept
{
    return "edi_n_delete_line_until_EOL" ;
}
bool EdiNDeleteLineUntilEOL::sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres) const
{
    auto del = [exres] {
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        using KeybrdEventer::pushup ;
        if(!ECBUtility::select_line_until_EOL(exres)) {
            if(!ECBUtility::copy_null()) {
                return false ;
            }
        }
        if(!pushup(VKC_LCTRL, VKC_X)) {
            return false ;
        }
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;
        return true ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return del() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return del() ;
    }
    return true ;
}


//EdiNDeleteAfter
struct EdiNDeleteAfter::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteAfter::EdiNDeleteAfter()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteAfter::~EdiNDeleteAfter() noexcept = default ;
EdiNDeleteAfter::EdiNDeleteAfter(EdiNDeleteAfter&&) noexcept = default ;
EdiNDeleteAfter& EdiNDeleteAfter::operator=(EdiNDeleteAfter&&) noexcept = default ;
const std::string EdiNDeleteAfter::sname() noexcept
{
    return "edi_n_delete_after" ;
}
bool EdiNDeleteAfter::sprocess(const bool first_call) const
{
    auto del = []{
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        using KeybrdEventer::pushup ;
        if(!pushup(VKC_LSHIFT, VKC_RIGHT)) {
            return false ;
        }
        if(!pushup(VKC_LCTRL, VKC_X)) {
            return false ;
        }
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;
        return true ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return del() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return del() ;
    }
    return true ;
}


//EdiNDeleteBefore
struct EdiNDeleteBefore::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNDeleteBefore::EdiNDeleteBefore()
: pimpl(std::make_unique<Impl>())
{}

EdiNDeleteBefore::~EdiNDeleteBefore() noexcept = default ;
EdiNDeleteBefore::EdiNDeleteBefore(EdiNDeleteBefore&&) noexcept = default ;
EdiNDeleteBefore& EdiNDeleteBefore::operator=(EdiNDeleteBefore&&) noexcept = default ;
const std::string EdiNDeleteBefore::sname() noexcept
{
    return "edi_n_delete_before" ;
}
bool EdiNDeleteBefore::sprocess(const bool first_call) const
{
    auto del = [] {
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        using KeybrdEventer::pushup ;
        if(!pushup(VKC_LSHIFT, VKC_LEFT)) {
            return false ;
        }
        if(!pushup(VKC_LCTRL, VKC_X)) {
            return false ;
        }
        ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;
        return true ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return del() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return del() ;
    }
    return true ;
}


//EdiDeleteMotionAndStartInsert
const std::string EdiDeleteMotionAndStartInsert::sname() noexcept
{
    return "edi_delete_motion_and_start_insert" ;
}
bool EdiDeleteMotionAndStartInsert::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    KeyLogger logger ;
    MSG msg ;
    POINT pos ;
    if(!GetCaretPos(&pos)) {
        return false ;
    }

    while(true) {
        Sleep(10) ;

        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
        }

        if(!logger.is_changed_code()) {
            logger.remove_from_back(1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(logger, KeyBinder::InvalidPolicy::UnbindedSystemKey)) {
            logger.remove_from_back(1) ;
            continue ;
        }

        if(auto func = KeyBinder::find_keybinds(logger)) {
            //skip itself
            if(func->name() == sname()) {
                continue ;
            }

            if(func->is_callable()) {
                std::cout << func->name() << " is matched\n" ;
                func->process(true) ;
                break ;
            }
        }
        else {
            logger.clear() ;
        }
    }

    return true ;
}


//EdiDeleteLinesAndStartInsert
const std::string EdiDeleteLinesAndStartInsert::sname() noexcept
{
    return "edi_delete_lines_and_start_insert" ;
}

bool EdiDeleteLinesAndStartInsert::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeyAbsorber::close_with_refresh()) {
        ERROR_PRINT("cannot refresh key-abosorber.") ;
        return false ;
    }

    if(auto res = TextAnalyzer::get_selected_text([] {
        if(!KeybrdEventer::pushup(VKC_HOME)) {
            return false ;
        }
        if(!KeybrdEventer::pushup(VKC_LSHIFT, VKC_END)) {
            return false ;
        }
        if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_INSERT)) {
            return false ;
        }
        return true ;
    })) {

        if(res.str.empty()) {
            return Change2EdiInsert::sprocess(true) ;
        }

        const auto pos = res.str.find_first_not_of(" \t") ; //position except for space or tab
        if(pos == std::string::npos) { //space only
            return Change2EdiEOLInsert::sprocess(true) ;
        }

        if(!KeybrdEventer::pushup(VKC_HOME)) {
            return false ;
        }

        for(int i = 0 ; i < static_cast<int>(pos) ; i ++) {
            if(!KeybrdEventer::pushup(VKC_RIGHT)) {
                return false ;
            }
        }
        if(!EdiDeleteUntilEOLAndStartInsert::sprocess(true, &res)) {
            return false ;
        }
    }

    return true ;
}


//EdiDeleteCharsAndStartInsert
const std::string EdiDeleteCharsAndStartInsert::sname() noexcept
{
    return "edi_delete_chars_and_start_insert" ;
}
bool EdiDeleteCharsAndStartInsert::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT)) {
        return false ;
    }
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    if(!Change2EdiInsert::sprocess(true)) {
        return false ;
    }

    return true ;
}


//EdiDeleteUntilEOLAndStartInsert
const std::string EdiDeleteUntilEOLAndStartInsert::sname() noexcept
{
    return "edi_delete_until_eol_and_start_insert" ;
}
bool EdiDeleteUntilEOLAndStartInsert::sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres)
{
    if(!first_call) return true ;

    if(!ECBUtility::select_line_until_EOL(exres)) {
        if(!ECBUtility::copy_null()) {
            return false ;
        }
    }
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    if(!Change2EdiInsert::sprocess(true)) {
        return false ;
    }

    return true ;
}
