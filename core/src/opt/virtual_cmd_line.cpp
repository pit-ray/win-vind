#include "opt/virtual_cmd_line.hpp"

#include <windows.h>

#include <chrono>
#include <unordered_map>

#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/display_text_painter.hpp"
#include "io/screen_metrics.hpp"
#include "mode.hpp"
#include "path.hpp"
#include "util/winwrap.hpp"

namespace
{
    POINT g_refresh_pos{0, 0} ;
    std::string g_outstr{} ;
    std::chrono::system_clock::time_point g_msg_start{} ;
    bool g_msg_showing = false ;
}

namespace vind
{
    struct VirtualCmdLine::Impl {
        DisplayTextPainter  dtp{25, FW_MEDIUM, "Consolas"} ;
        int x = 0 ;
        int y = 0 ;
        int extra = 0 ;
        std::chrono::seconds fadeout_time{} ;
    } ;

    VirtualCmdLine::VirtualCmdLine()
    : pimpl(std::make_unique<Impl>())
    {}
    VirtualCmdLine::~VirtualCmdLine() noexcept                  = default ;
    VirtualCmdLine::VirtualCmdLine(VirtualCmdLine&&)            = default ;
    VirtualCmdLine& VirtualCmdLine::operator=(VirtualCmdLine&&) = default ;

    std::string VirtualCmdLine::sname() noexcept {
        return "virtual_cmd_line" ;
    }

    void VirtualCmdLine::do_enable() const {
        reset() ;
        pimpl->dtp.set_font(
                gparams::get_l("cmd_font_size"),
                gparams::get_l("cmd_font_weight"),
                gparams::get_s("cmd_font_name")) ;

        pimpl->dtp.set_text_color(gparams::get_s("cmd_font_color")) ;
        pimpl->dtp.set_back_color(gparams::get_s("cmd_font_bkcolor")) ;

        auto pos = gparams::get_s("cmd_pos") ;
        auto xma = gparams::get_i("cmd_xmargin") ;
        auto yma = gparams::get_i("cmd_ymargin") ;

        RECT rect ;
        screenmetrics::get_primary_metrics(&rect) ;

        auto width  = screenmetrics::width(rect) ;
        auto height = screenmetrics::height(rect) ;

        constexpr auto midxbuf = 256 ;

        std::unordered_map<std::string, std::pair<int, int>> pos_list {
            {"UpperLeft",  {xma,                    yma}},
            {"UpperMid",   {width / 2 - midxbuf,    yma}},
            {"UpperRight", {width - xma - midxbuf,  yma}},
            {"MidLeft",    {xma,                    height / 2}},
            {"Center",     {width / 2 - midxbuf,    height / 2}},
            {"MidRight",   {width - xma - midxbuf,  height / 2}},
            {"LowerLeft",  {xma,                    height - yma}},
            {"LowerMid",   {width / 2 - midxbuf,    height - yma}},
            {"LowerRight", {width - xma - midxbuf,  height - yma}}
        } ;
        try {
            const auto& p = pos_list.at(gparams::get_s("cmd_pos")) ;
            pimpl->x = p.first ;
            pimpl->y = p.second ;
        }
        catch(const std::out_of_range& e) {
            const auto& p = pos_list.at("LowerMid") ;
            pimpl->x = p.first ;
            pimpl->y = p.second ;
            PRINT_ERROR(std::string(e.what()) + "in " + path::SETTINGS() + ", " + gparams::get_s("cmd_pos") + "is invalid syntax.") ;
        }

        pimpl->extra = gparams::get_i("cmd_font_extra") ;
        pimpl->fadeout_time = std::chrono::seconds(gparams::get_i("cmd_fadeout_time")) ;

        g_refresh_pos.x = pimpl->x ;
        g_refresh_pos.y = pimpl->y ;
    }

    void VirtualCmdLine::do_disable() const {
    }

    void VirtualCmdLine::cout(std::string&& str) {
        if(g_msg_showing) reset() ;
        g_outstr = std::move(str) ;
    }
    void VirtualCmdLine::cout(const std::string& str) {
        g_outstr = str ;
    }

    void VirtualCmdLine::msgout(const std::string& str) noexcept {
        if(str.empty()) return ;
        g_outstr = std::move(str) ;
        g_msg_start = std::chrono::system_clock::now() ;
        g_msg_showing = true ;
    }

    void VirtualCmdLine::refresh() {
        util::refresh_display(WindowFromPoint(g_refresh_pos)) ;
    }

    void VirtualCmdLine::clear() {
        g_outstr.clear() ;
    }

    void VirtualCmdLine::reset() {
        g_msg_showing = false ;
        if(!g_outstr.empty()) {
            clear() ;
            refresh() ;
        }
    }

    void VirtualCmdLine::do_process() const {
        if(g_outstr.empty()) return ;
        if(g_msg_showing) {
            if(std::chrono::system_clock::now() - g_msg_start > pimpl->fadeout_time) {
                reset() ;
                return ;
            }
        }

        pimpl->dtp.draw(g_outstr, pimpl->x, pimpl->y, pimpl->extra) ;
    }
}
