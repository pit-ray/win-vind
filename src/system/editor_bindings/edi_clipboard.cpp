#include "edi_clipboard.hpp"

#include <iostream> //for debug
#include <memory>
#include <windows.h>

#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "simpl_text_selecter.hpp"
#include "msg_logger.hpp"
#include "keystroke_repeater.hpp"
#include "edi_change_mode.hpp"

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
bool EdiNCopyLine::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }
    using KeybrdEventer::pushup ;

    if(!pushup(VKC_HOME)) {
        return false ;
    }
    if(!pushup(VKC_LSHIFT, VKC_END)) {
        return false ;
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
        if(!pushup(VKC_BKSPACE)) {
            return false ;
        }
        if(!pushup(VKC_LCTRL, VKC_RIGHT)) {
            return false ;
        }

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
bool EdiNDeleteLine::sprocess(const bool first_call) const
{
    auto del = [] {
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        if(!SimplTextSelecter::select_line_BOL2EOL()) {
            return false ;
        }

        using KeybrdEventer::pushup ;
        if(!pushup(VKC_HOME)) {
            return false ;
        }
        if(!pushup(VKC_LSHIFT, VKC_END)) {
            return false ;
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
bool EdiNDeleteLineUntilEOL::sprocess(const bool first_call) const
{
    auto del = [] {
        if(ECBUtility::delete_line_when_selecting()) {
            return true ;
        }

        //not select
        using KeybrdEventer::pushup ;
        if(!pushup(VKC_LSHIFT, VKC_END)) {
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