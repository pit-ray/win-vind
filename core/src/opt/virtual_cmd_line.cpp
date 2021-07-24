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
        DisplayTextPainter  dtp_{25, FW_MEDIUM, "Consolas"} ;
        int x_ = 0 ;
        int y_ = 0 ;
        int extra_ = 0 ;
        std::chrono::seconds fadeout_time_{} ;
    } ;

    VirtualCmdLine::VirtualCmdLine()
    : OptionCreator("vcmdline"),
      pimpl(std::make_unique<Impl>())
    {}
    VirtualCmdLine::~VirtualCmdLine() noexcept                  = default ;
    VirtualCmdLine::VirtualCmdLine(VirtualCmdLine&&)            = default ;
    VirtualCmdLine& VirtualCmdLine::operator=(VirtualCmdLine&&) = default ;


    void VirtualCmdLine::do_enable() const {
        reset() ;
        pimpl->dtp_.set_font(
                gparams::get_l("cmd_fontsize"),
                gparams::get_l("cmd_fontweight"),
                gparams::get_s("cmd_fontname")) ;

        pimpl->dtp_.set_text_color(gparams::get_s("cmd_fontcolor")) ;
        pimpl->dtp_.set_back_color(gparams::get_s("cmd_bgcolor")) ;

        auto pos = gparams::get_s("cmd_roughpos") ;
        auto xma = gparams::get_i("cmd_xmargin") ;
        auto yma = gparams::get_i("cmd_ymargin") ;

        auto rect = screenmetrics::get_primary_metrics() ;

        auto w = rect.width() ;
        auto h = rect.height() ;

        constexpr auto midxbuf = 256 ;

        std::unordered_map<std::string, std::pair<int, int>> pos_list {
            {"UpperLeft",  {xma,                yma}},
            {"UpperMid",   {w / 2 - midxbuf,    yma}},
            {"UpperRight", {w - xma - midxbuf,  yma}},
            {"MidLeft",    {xma,                h / 2}},
            {"Center",     {w / 2 - midxbuf,    h / 2}},
            {"MidRight",   {w - xma - midxbuf,  h / 2}},
            {"LowerLeft",  {xma,                h - yma}},
            {"LowerMid",   {w / 2 - midxbuf,    h - yma}},
            {"LowerRight", {w - xma - midxbuf,  h - yma}}
        } ;
        try {
            const auto& p = pos_list.at(gparams::get_s("cmd_roughpos")) ;
            pimpl->x_ = p.first ;
            pimpl->y_ = p.second ;
        }
        catch(const std::out_of_range& e) {
            const auto& p = pos_list.at("LowerMid") ;
            pimpl->x_ = p.first ;
            pimpl->y_ = p.second ;
            PRINT_ERROR(std::string(e.what()) + "in " + path::SETTINGS() + ", " + gparams::get_s("cmd_roughpos") + "is invalid syntax.") ;
        }

        pimpl->extra_ = gparams::get_i("cmd_fontextra") ;
        pimpl->fadeout_time_ = std::chrono::seconds(gparams::get_i("cmd_fadeout")) ;

        g_refresh_pos.x = pimpl->x_ ;
        g_refresh_pos.y = pimpl->y_ ;
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

    void VirtualCmdLine::msgout(const std::string& str) {
        if(str.empty()) return ;
        if(!g_outstr.empty()) {
            g_outstr = str ;
            refresh() ; // reset pixel
        }
        else {
            g_outstr = str ;
        }
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
            if(std::chrono::system_clock::now() - g_msg_start > pimpl->fadeout_time_) {
                reset() ;
                return ;
            }
        }

        pimpl->dtp_.draw(g_outstr, pimpl->x_, pimpl->y_, pimpl->extra_) ;
    }
}
