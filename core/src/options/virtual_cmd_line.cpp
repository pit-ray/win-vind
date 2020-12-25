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

static POINT g_refresh_pos = {0, 0} ;

struct VirtualCmdLine::Impl
{
    LOGFONT lf{} ;
    COLORREF color{RGB(0, 0, 0)} ;
    COLORREF bkcolor{RGB(0, 0, 0)} ;
    int x     = 0 ;
    int y     = 0 ;
    int extra = 0 ;

    std::chrono::seconds fadeout_time{} ;
} ;

VirtualCmdLine::VirtualCmdLine()
: pimpl(std::make_unique<Impl>())
{
    //default setting
    pimpl->lf.lfHeight         = 25 ;
    pimpl->lf.lfWidth          = 0 ;
    pimpl->lf.lfEscapement     = 0 ;
    pimpl->lf.lfOrientation    = 0 ;
    pimpl->lf.lfWeight         = FW_MEDIUM ;
    pimpl->lf.lfItalic         = FALSE ;
    pimpl->lf.lfUnderline      = FALSE ;
    pimpl->lf.lfStrikeOut      = FALSE ;
    pimpl->lf.lfCharSet        = ANSI_CHARSET ;
    pimpl->lf.lfOutPrecision   = OUT_TT_ONLY_PRECIS ;
    pimpl->lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS ;
    pimpl->lf.lfQuality        = ANTIALIASED_QUALITY ;
    pimpl->lf.lfPitchAndFamily = 0 ;
    pimpl->lf.lfFaceName[0]    = '\0' ;
}
VirtualCmdLine::~VirtualCmdLine() noexcept                  = default ;
VirtualCmdLine::VirtualCmdLine(VirtualCmdLine&&)            = default ;
VirtualCmdLine& VirtualCmdLine::operator=(VirtualCmdLine&&) = default ;

const string VirtualCmdLine::sname() noexcept
{
    return "virtual_cmd_line" ;
}

void VirtualCmdLine::do_enable() const
{
    reset() ;
    pimpl->lf.lfHeight = iParams::get_l("cmd_font_size") ;
    pimpl->lf.lfWeight = iParams::get_l("cmd_font_weight") ;

    pimpl->color   = Utility::hex2COLORREF(iParams::get_s("cmd_font_color")) ;
    pimpl->bkcolor = Utility::hex2COLORREF(iParams::get_s("cmd_font_bkcolor")) ;

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

    g_refresh_pos.x = pimpl->x ;
    g_refresh_pos.y = pimpl->y ;
}

void VirtualCmdLine::do_disable() const
{
}

void VirtualCmdLine::cout(std::string&& str) noexcept
{
    if(msg_showing) reset() ;
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

void VirtualCmdLine::refresh() {
    if(!InvalidateRect(WindowFromPoint(g_refresh_pos), NULL, TRUE)) {
        throw RUNTIME_EXCEPT(" failed refresh display") ;
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

void VirtualCmdLine::do_process() const
{
    if(outstr.empty()) return ;
    if(msg_showing) {
        if(std::chrono::system_clock::now() - msg_start > pimpl->fadeout_time) {
            reset() ;
            return ;
        }
    }

    auto delete_hdc = [] (HDC h) {
        if(h != nullptr) DeleteDC(h) ;
    } ;
    std::unique_ptr<HDC__, decltype(delete_hdc)> hdc(
            CreateDCA("DISPLAY", NULL, NULL, NULL), delete_hdc) ;
    if(!hdc) {
        throw RUNTIME_EXCEPT("CreateDC") ;
    }

    auto delete_font = [] (HFONT f) {
        if(f != nullptr) DeleteObject(f) ;
    } ;
    std::unique_ptr<HFONT__, decltype(delete_font)> font(
            CreateFontIndirect(&pimpl->lf), delete_font) ;
    if(!font) {
        throw RUNTIME_EXCEPT("CreateFontIndirectA") ;
    }

    if(!SelectObject(hdc.get(), font.get())) {
        throw RUNTIME_EXCEPT("SelectObject") ;
    }

    if(SetBkColor(hdc.get(), pimpl->bkcolor) == CLR_INVALID) {
        throw RUNTIME_EXCEPT("SetBkColor") ;
    }

    if(SetTextColor(hdc.get(), pimpl->color) == CLR_INVALID) {
        throw RUNTIME_EXCEPT("SetTextColor") ;
    }

    if(SetTextCharacterExtra(hdc.get(), pimpl->extra) == static_cast<int>(0x80000000)) {
        throw RUNTIME_EXCEPT("SetTextCharacterExtra") ;
    }

    if(!TextOutA(hdc.get(), pimpl->x, pimpl->y, outstr.c_str(), lstrlenA(outstr.c_str()))) {
        throw RUNTIME_EXCEPT("TextOutA") ;
    }
}
