#include "display_cmd.hpp"
#include "ini_parser.hpp"
#include "utility.hpp"
#include "default_config.hpp"
#include "key_binder.hpp"
#include "mode_manager.hpp"

#include <windows.h>

using namespace std ;

struct DisplayCmd::Impl
{
    const KeyBinder* const pkb ;
    LOGFONT lf ;
    COLORREF color ;
    COLORREF bkcolor ;
    int x ;
    int y ;
    int extra ;

    explicit Impl(const KeyBinder* const ptr2kb)
    : pkb(ptr2kb),
      lf(),
      color(RGB(0, 0, 0)),
      bkcolor(RGB(0, 0, 0)),
      x(0),
      y(0),
      extra(0)
    {}
} ;

DisplayCmd::DisplayCmd(const KeyBinder* const pkb)
: pimpl(make_unique<Impl>(pkb))
{
    //default setting
    pimpl->lf.lfHeight = 25 ;
    pimpl->lf.lfWidth = 0 ;
    pimpl->lf.lfEscapement = 0 ;
    pimpl->lf.lfOrientation = 0 ;
    pimpl->lf.lfWeight = FW_MEDIUM ;
    pimpl->lf.lfItalic = FALSE ;
    pimpl->lf.lfUnderline = FALSE ;
    pimpl->lf.lfStrikeOut = FALSE ;
    pimpl->lf.lfCharSet = ANSI_CHARSET ;
    pimpl->lf.lfOutPrecision = OUT_TT_ONLY_PRECIS ;
    pimpl->lf.lfClipPrecision = CLIP_DEFAULT_PRECIS ;
    pimpl->lf.lfQuality = ANTIALIASED_QUALITY ;
    pimpl->lf.lfPitchAndFamily = 0 ;
    pimpl->lf.lfFaceName[0] = '\0' ;
}

const string DisplayCmd::name() const noexcept
{
    return "display_cmd" ;
}

unique_ptr<DynamicOption> DisplayCmd::create(const KeyBinder* const pkb)
{
    return std::move(make_unique<DisplayCmd>(pkb)) ;
}

namespace DiCmUtility
{
    inline static const auto hex2COLOREF(string hex) noexcept {
        if(hex.front() == '#') {
            hex.erase(0, 1) ;
        }

        unsigned char r = 0 ;
        unsigned char g = 0 ;
        unsigned char b = 0 ;

        if(hex.length() == 6) {
            auto r_hex = hex.substr(0, 2) ;
            auto g_hex = hex.substr(2, 2) ;
            auto b_hex = hex.substr(4, 2) ;
            r = static_cast<unsigned char>(strtol(r_hex.c_str(), nullptr, 16)) ;
            g = static_cast<unsigned char>(strtol(g_hex.c_str(), nullptr, 16)) ;
            b = static_cast<unsigned char>(strtol(b_hex.c_str(), nullptr, 16)) ;
        }

        return RGB(r, g, b) ;
    }
}
bool DisplayCmd::do_enable() const noexcept
{
    try {
        const auto pt = INIParser::load_config(Path::CONFIG_INI()) ;

        pimpl->lf.lfHeight = pt.get_optional<int>("Property.cmd_font_size").get() ;
        pimpl->lf.lfWeight = pt.get_optional<int>("Property.cmd_font_weight").get() ;

        using namespace DiCmUtility ;
        pimpl->color = hex2COLOREF(pt.get_optional<string>("Property.cmd_font_color").get()) ;
        pimpl->bkcolor = hex2COLOREF(pt.get_optional<string>("Property.cmd_font_bkcolor").get()) ;

        const auto pos = pt.get_optional<string>("Property.cmd_pos").get() ;
        const auto xma = pt.get_optional<int>("Property.cmd_xmargin").get() ;
        const auto yma = pt.get_optional<int>("Property.cmd_ymargin").get() ;

        static const auto xmet = GetSystemMetrics(SM_CXSCREEN) ;
        static const auto ymet = GetSystemMetrics(SM_CYSCREEN) ;

        constexpr auto mid_xbuf = 256 ;

        if(pos == "UpperLeft") {
            pimpl->x = xma ;
            pimpl->y = yma ;
        }
        else if(pos == "UpperMid") {
            pimpl->x = xmet / 2 - mid_xbuf ;
            pimpl->y = yma ;
        }
        else if(pos == "UpperRight") {
            pimpl->x = xmet - xma - mid_xbuf ;
            pimpl->y = yma ;
        }

        else if(pos == "MidLeft") {
            pimpl->x = xma ;
            pimpl->y = ymet / 2 ;
        }
        else if(pos == "Center") {
            pimpl->x = xmet / 2 - mid_xbuf ;
            pimpl->y = ymet / 2 ;
        }
        else if(pos == "MidRight") {
            pimpl->x = xmet - xma - mid_xbuf ;
            pimpl->y = ymet / 2 ;
        }

        else if(pos == "LowerLeft") {
            pimpl->x = xma ;
            pimpl->y = ymet - yma ;
        }
        else if(pos == "LowerMid") {
            pimpl->x = xmet / 2 - mid_xbuf ;
            pimpl->y = ymet - yma ;
        }
        else if(pos == "LowerRight") {
            pimpl->x = xmet - xma - mid_xbuf ;
            pimpl->y = ymet - yma ;
        }
        else {
            pimpl->x = xmet / 2 - mid_xbuf ;
            pimpl->y = ymet - yma ;
        }

        pimpl->extra = pt.get_optional<int>("Property.cmd_font_extra").get() ;
    }
    catch(...) {
        return false ;
    }
    return true ;
}

bool DisplayCmd::do_disable() const noexcept
{
    return true ;
}

namespace DiCmUtility
{
    inline static void write_error(const string& func_name) noexcept {
        Logger::error_stream << "[Error] windows.h: "\
        << GetLastError() << (" (display_cmd.cpp::DisplayCmd::do_process::" + func_name) << ")\n" ;
    }
}

bool DisplayCmd::do_process() const
{
    using namespace DiCmUtility ;
    if(ModeManager::get_mode() != ModeManager::Mode::Command) {
        return true ;
    }

    const auto str = pimpl->pkb->get_logger_str() ;
    if(str.empty()) return true ;

    auto hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
    if(!hdc) {
        write_error("CreateDC") ;
        return false ;
    }

    auto font = CreateFontIndirect(&pimpl->lf) ;
    if(!font) {
        write_error("CreateFontIndirectA") ;
        return false ;
    }

    if(!SelectObject(hdc, font)) {
        write_error("SelectObject") ;
        return false ;
    }

    if(SetBkColor(hdc, pimpl->bkcolor) == CLR_INVALID) {
        write_error("SetBkColor") ;
        return false ;
    }

    if(SetTextColor(hdc, pimpl->color) == CLR_INVALID) {
        write_error("SetTextColor") ;
        return false ;
    }

    if(SetTextCharacterExtra(hdc, pimpl->extra) == static_cast<int>(0x80000000)) {
        write_error("SetTextCharacterExtra") ;
        return false ;
    }

    auto cstr = str.c_str() ;
    if(!TextOutA(hdc, pimpl->x, pimpl->y, cstr, lstrlenA(cstr))) {
        write_error("TextOutA") ;
        return false ;
    }

    if(!DeleteDC(hdc)) {
        write_error("DeleteDC") ;
        return false ;
    }

    if(!DeleteObject(font)) {
        write_error("DeleteObject") ;
        return false ;
    }

    return true ;
}