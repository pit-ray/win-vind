#include "screen_metrics.hpp"
#include "virtual_cmd_line.hpp"

#include <windows.h>
#include <unordered_map>

#include "utility.hpp"
#include "mode_manager.hpp"
#include "path.hpp"
#include "i_params.hpp"

using namespace std ;
std::string VirtualCmdLine::outstr{} ;
std::chrono::system_clock::time_point VirtualCmdLine::msg_start{} ;
bool VirtualCmdLine::msg_showing = false ;

namespace VCLUtility
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
using namespace VCLUtility ;

struct VirtualCmdLine::Impl
{
    LOGFONT lf{} ;
    COLORREF color{RGB(0, 0, 0)} ;
    COLORREF bkcolor{RGB(0, 0, 0)} ;
    int x = 0 ;
    int y = 0 ;
    int extra = 0 ;

    std::chrono::seconds fadeout_time{} ;
} ;

VirtualCmdLine::VirtualCmdLine()
: pimpl(std::make_unique<Impl>())
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
VirtualCmdLine::~VirtualCmdLine() noexcept = default ;
VirtualCmdLine::VirtualCmdLine(VirtualCmdLine&&) noexcept = default ;
VirtualCmdLine& VirtualCmdLine::operator=(VirtualCmdLine&&) noexcept = default ;

const string VirtualCmdLine::sname() noexcept
{
    return "virtual_cmd_line" ;
}

bool VirtualCmdLine::do_enable() const noexcept
{
    reset() ;
    pimpl->lf.lfHeight = iParams::get_l("cmd_font_size") ;
    pimpl->lf.lfWeight = iParams::get_l("cmd_font_weight") ;

    pimpl->color = hex2COLOREF(iParams::get_s("cmd_font_color")) ;
    pimpl->bkcolor = hex2COLOREF(iParams::get_s("cmd_font_bkcolor")) ;

    const auto pos = iParams::get_s("cmd_pos") ;
    const auto xma = iParams::get_i("cmd_xmargin") ;
    const auto yma = iParams::get_i("cmd_ymargin") ;

    static const ScreenMetrics met{} ;

    constexpr auto midxbuf = 256 ;

    const std::unordered_map<std::string, std::pair<int, int>> pos_list {
        {"UpperLeft",  {xma, yma}},
        {"UpperMid",   {met.primary_width() / 2 - midxbuf, yma}},
        {"UpperRight", {met.primary_width() - xma - midxbuf, yma}},
        {"MidLeft",    {xma, met.primary_height() / 2}},
        {"Center",     {met.primary_width() / 2 - midxbuf, met.primary_height() / 2}},
        {"MidRight",   {met.primary_width() - xma - midxbuf, met.primary_height() / 2}},
        {"LowerLeft",  {xma, met.primary_height() - yma}},
        {"LowerMid",   {met.primary_width() / 2 - midxbuf, met.primary_height() - yma}},
        {"LowerRight", {met.primary_width() - xma - midxbuf, met.primary_height() - yma}}
    } ;
    try {
        const auto& p = pos_list.at(iParams::get_s("cmd_pos")) ;
        pimpl->x = p.first ;
        pimpl->y = p.second ;
    }
    catch(const std::out_of_range& e) {
        const auto& p = pos_list.at("LowerMid") ;
        pimpl->x = p.first ;
        pimpl->y = p.second ;
        ERROR_PRINT(std::string(e.what()) + "in " + Path::SETTINGS() + ", " + iParams::get_s("cmd_pos") + "is invalid syntax.") ;
    }

    pimpl->extra = iParams::get_i("cmd_font_extra") ;
    pimpl->fadeout_time = std::chrono::seconds(iParams::get_i("cmd_fadeout_time")) ;
    return true ;
}

bool VirtualCmdLine::do_disable() const noexcept
{
    return true ;
}

void VirtualCmdLine::cout(std::string&& str) noexcept
{
    outstr = std::move(str) ;
}
void VirtualCmdLine::cout(const std::string& str) noexcept
{
    outstr = str ;
}

void VirtualCmdLine::msgout(std::string str) noexcept
{
    if(str.empty()) return ;

    outstr = std::move(str) ;
    msg_start = std::chrono::system_clock::now() ;
    msg_showing = true ;
}

void VirtualCmdLine::refresh() noexcept {
    if(!InvalidateRect(NULL, NULL, TRUE)) {
        WIN_ERROR_PRINT(" failed refresh display") ;
    }
}

void VirtualCmdLine::clear() noexcept
{
    outstr.clear() ;
}

void VirtualCmdLine::reset() noexcept
{
    clear() ;
    msg_showing = false ;
    refresh() ;
}

bool VirtualCmdLine::do_process() const
{
    if(outstr.empty()) return true ;

    if(msg_showing) {
        if(std::chrono::system_clock::now() - msg_start > pimpl->fadeout_time) {
            reset() ;
            return true ;
        }
    }

    auto hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
    if(!hdc) {
        WIN_ERROR_PRINT("CreateDC") ;
        return false ;
    }

    auto font = CreateFontIndirect(&pimpl->lf) ;
    if(!font) {
        WIN_ERROR_PRINT("CreateFontIndirectA") ;
        return false ;
    }

    if(!SelectObject(hdc, font)) {
        WIN_ERROR_PRINT("SelectObject") ;
        return false ;
    }

    if(SetBkColor(hdc, pimpl->bkcolor) == CLR_INVALID) {
        WIN_ERROR_PRINT("SetBkColor") ;
        return false ;
    }

    if(SetTextColor(hdc, pimpl->color) == CLR_INVALID) {
        WIN_ERROR_PRINT("SetTextColor") ;
        return false ;
    }

    if(SetTextCharacterExtra(hdc, pimpl->extra) == static_cast<int>(0x80000000)) {
        WIN_ERROR_PRINT("SetTextCharacterExtra") ;
        return false ;
    }

    if(!TextOutA(hdc, pimpl->x, pimpl->y, outstr.c_str(), lstrlenA(outstr.c_str()))) {
        WIN_ERROR_PRINT("TextOutA") ;
        return false ;
    }

    if(!DeleteDC(hdc)) {
        WIN_ERROR_PRINT("DeleteDC") ;
        return false ;
    }

    if(!DeleteObject(font)) {
        WIN_ERROR_PRINT("DeleteObject") ;
        return false ;
    }

    return true ;
}
