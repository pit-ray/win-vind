#include "edi_clipboard.hpp"

#include <iostream> //for debug
#include <memory>
#include <windows.h>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "simpl_text_selecter.hpp"
#include "msg_logger.hpp"

namespace ECBUtility
{
    enum class _RegisteredType : unsigned char {
        None,
        Chars,
        Lines,
    } ;

    static _RegisteredType _rgtype{_RegisteredType::None} ;
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

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_INSERT)) {
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

    change_mode(Mode::EdiNormal) ;
    if(!SimplTextSelecter::is_unselect()) {
        return false ;
    }

    return true ;
}


//EdiNCopyLine (EdiNormal only)
const std::string EdiNCopyLine::sname() noexcept
{
    return "edi_n_copy_line" ;
}
bool EdiNCopyLine::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    using KeybrdEventer::is_pushup ;

    if(!is_pushup(VKC_HOME)) {
        return false ;
    }
    if(!is_pushup(VKC_LSHIFT, VKC_END)) {
        return false ;
    }
    if(!is_pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }
    if(!is_pushup(VKC_END)) {
        return false ;
    }

    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;

    return true ;
}


//EdiNPasteAfter (EdiNormal or EdiVisual)
const std::string EdiNPasteAfter::sname() noexcept
{
    return "edi_n_paste_after" ;
}
bool EdiNPasteAfter::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    using ECBUtility::_RegisteredType ;
    using ECBUtility::_rgtype ;

    if(_rgtype == _RegisteredType::Chars) {
        if(!KeybrdEventer::is_pushup(VKC_LSHIFT, VKC_INSERT)) {
            return false ;
        }

        ModeManager::change_mode(ModeManager::Mode::EdiNormal) ;
        return true ;
    }

    if(_rgtype == _RegisteredType::Lines) {
        if(ModeManager::is_edi_visual()) {
            return true ;
        }

        using KeybrdEventer::is_pushup ;
        if(!is_pushup(VKC_END)) {
            return false ;
        }

        //It might be double newlines in some applications distinglishing EOL.
        //In other words, creating a empty line.
        if(!is_pushup(VKC_ENTER)) {
            return false ;
        }
        if(!is_pushup(VKC_LSHIFT, VKC_INSERT)) {
            return false ;
        }

        return true ;
    }
    return true ;
}


//EdiNPasteBefore
const std::string EdiNPasteBefore::sname() noexcept
{
    return "edi_n_paste_before" ;
}
bool EdiNPasteBefore::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    using ECBUtility::_RegisteredType ;
    using ECBUtility::_rgtype ;

    if(_rgtype == _RegisteredType::Chars) {
        if(!KeybrdEventer::is_pushup(VKC_LSHIFT, VKC_INSERT)) {
            return false ;
        }

        ModeManager::change_mode(ModeManager::Mode::EdiNormal) ;
        return true ;
    }

    if(_rgtype == _RegisteredType::Lines) {
        if(ModeManager::is_edi_visual()) {
            return true ;
        }

        using KeybrdEventer::is_pushup ;
        if(!is_pushup(VKC_HOME)) {
            return false ;
        }
        if(!is_pushup(VKC_ENTER)) {
            return false ;
        }
        if(!is_pushup(VKC_UP)) {
            return false ;
        }
        if(!is_pushup(VKC_LSHIFT, VKC_INSERT)) {
            return false ;
        }
        return true ;
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

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_X)) {
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

    change_mode(Mode::EdiNormal) ;
    return true ;
}


namespace ECBUtility
{

    inline static bool _is_delete_line() {
        using KeybrdEventer::is_pushup ;
        if(!is_pushup(VKC_LCTRL, VKC_X)) {
            return false ;
        }
        if(!is_pushup(VKC_BKSPACE)) {
            return false ;
        }
        if(!is_pushup(VKC_LCTRL, VKC_RIGHT)) {
            return false ;
        }

        return true ;
    }

    inline static bool _is_delete_line_when_selecting() {
        using namespace ModeManager ;
        const auto mode = get_mode() ;
        if(mode == Mode::EdiVisual) {
            if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_X)) {
                return false ;
            }

            change_mode(Mode::EdiNormal) ;
            _rgtype = _RegisteredType::Chars ;
            return true ;
        }
        if(mode == Mode::EdiLineVisual) {
            if(!ECBUtility::_is_delete_line()) {
                return false ;
            }

            change_mode(Mode::EdiNormal) ;
            _rgtype = _RegisteredType::Lines ;
            return true ;
        }

        return false ;
    }
}

//EdiNDeleteLine
const std::string EdiNDeleteLine::sname() noexcept
{
    return "edi_n_delete_line" ;
}
bool EdiNDeleteLine::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    if(ECBUtility::_is_delete_line_when_selecting()) {
        return true ;
    }

    //not select
    if(!SimplTextSelecter::is_select_line_BOL2EOL()) {
        return false ;
    }

    using KeybrdEventer::is_pushup ;
    if(!is_pushup(VKC_HOME)) {
        return false ;
    }
    if(!is_pushup(VKC_LSHIFT, VKC_END)) {
        return false ;
    }
    if(!ECBUtility::_is_delete_line()) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Lines ;

    return true ;
}


//EdiNDeleteLineUntilEOL
const std::string EdiNDeleteLineUntilEOL::sname() noexcept
{
    return "edi_n_delete_line_until_EOL" ;
}
bool EdiNDeleteLineUntilEOL::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    if(ECBUtility::_is_delete_line_when_selecting()) {
        return true ;
    }

    //not select
    using KeybrdEventer::is_pushup ;
    if(!is_pushup(VKC_LSHIFT, VKC_END)) {
        return false ;
    }
    if(!is_pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    return true ;
}


//EdiNDeleteAfter
const std::string EdiNDeleteAfter::sname() noexcept
{
    return "edi_n_delete_after" ;
}
bool EdiNDeleteAfter::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    if(ECBUtility::_is_delete_line_when_selecting()) {
        return true ;
    }

    //not select
    using KeybrdEventer::is_pushup ;
    if(!is_pushup(VKC_LSHIFT, VKC_RIGHT)) {
        return false ;
    }
    if(!is_pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    return true ;
}


//EdiNDeleteBefore
const std::string EdiNDeleteBefore::sname() noexcept
{
    return "edi_n_delete_before" ;
}
bool EdiNDeleteBefore::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    if(ECBUtility::_is_delete_line_when_selecting()) {
        return true ;
    }

    //not select
    using KeybrdEventer::is_pushup ;
    if(!is_pushup(VKC_LSHIFT, VKC_LEFT)) {
        return false ;
    }
    if(!is_pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }
    ECBUtility::_rgtype = ECBUtility::_RegisteredType::Chars ;

    return true ;
}