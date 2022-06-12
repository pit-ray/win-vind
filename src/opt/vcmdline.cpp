#include "opt/vcmdline.hpp"

#include <windows.h>

#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "core/errlogger.hpp"
#include "core/mode.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"
#include "util/box2d.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/display_text_painter.hpp"
#include "util/point2d.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace
{
    enum class ProjectionMethod {
        PRIMARY,
        ALL,
        FIXED,
        ACTIVE,
    } ;

    using namespace vind ;

    struct Projector {
        const int out_margin_x_ ;
        const int out_margin_y_ ;
        const int area_bias_x_ ;

        template <typename T1>
        Projector(T1 out_margin_x, T1 out_margin_y)
        : out_margin_x_(static_cast<int>(out_margin_x)),
          out_margin_y_(static_cast<int>(out_margin_y)),
          area_bias_x_(-256)
        {}

        template <typename T1, typename T2>
        Projector(T1 out_margin_x, T1 out_margin_y, T2 area_bias_x)
        : out_margin_x_(static_cast<int>(out_margin_x)),
          out_margin_y_(static_cast<int>(out_margin_y)),
          area_bias_x_(static_cast<int>(area_bias_x))
        {}

        virtual ~Projector() noexcept = default ;

        virtual std::tuple<int, int> project(int origin_x, int origin_y, int w, int h) = 0 ;
    } ;

    struct UpperLeftProjector : public Projector {
        template <typename... Args>
        UpperLeftProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int UNUSED(w),
                int UNUSED(h)) override {
            return {
                origin_x + out_margin_x_,
                origin_y + out_margin_y_
            } ;
        }
    } ;

    struct UpperMidProjector : public Projector {
        template <typename... Args>
        UpperMidProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int UNUSED(h)) override {
            return {
                origin_x + w / 2 + area_bias_x_,
                origin_y + out_margin_y_
            } ;
        }
    } ;

    struct UpperRightProjector : public Projector {
        template <typename... Args>
        UpperRightProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int UNUSED(h)) override {
            return {
                origin_x + w - out_margin_x_ + area_bias_x_,
                origin_y + out_margin_y_
            } ;
        }
    } ;

    struct MidLeftProjector : public Projector {
        template <typename... Args>
        MidLeftProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int UNUSED(w),
                int h) override {
            return {
                origin_x + out_margin_x_,
                origin_y + h / 2
            } ;
        }
    } ;

    struct CenterProjector : public Projector {
        template <typename... Args>
        CenterProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int h) override {
            return {
                origin_x + w / 2 + area_bias_x_,
                origin_y + h / 2
            } ;
        }
    } ;

    struct MidRightProjector : public Projector {
        template <typename... Args>
        MidRightProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int h) override {
            return {
                origin_x + w - out_margin_x_ + area_bias_x_,
                origin_y + h / 2
            } ;
        }
    } ;

    struct LowerLeftProjector : public Projector {
        template <typename... Args>
        LowerLeftProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int UNUSED(w),
                int h) override {
            return {
                origin_x + out_margin_x_,
                origin_y + h - out_margin_y_
            } ;
        }
    } ;

    struct LowerMidProjector : public Projector {
        template <typename... Args>
        LowerMidProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int h) override {
            return {
                origin_x + w / 2 + area_bias_x_,
                origin_y + h - out_margin_y_
            } ;
        }
    } ;

    struct LowerRightProjector : public Projector {
        template <typename... Args>
        LowerRightProjector(Args&&... args)
        : Projector(args...)
        {}

        std::tuple<int, int> project(
                int origin_x,
                int origin_y,
                int w,
                int h) override {
            return {
                origin_x + w - out_margin_x_ + area_bias_x_,
                origin_y + h - out_margin_y_
            } ;
        }
    } ;

    template <typename String, typename... Args>
    std::unique_ptr<Projector> create_projector(String&& name, Args&&... args) {
        auto fmt_name = util::A2a(std::forward<String>(name)) ;
        if(fmt_name == "upperleft") {
            return std::make_unique<UpperLeftProjector>(args...) ;
        }
        if(fmt_name == "uppermid") {
            return std::make_unique<UpperMidProjector>(args...) ;
        }
        if(fmt_name == "upperright") {
            return std::make_unique<UpperRightProjector>(args...) ;
        }
        if(fmt_name == "midleft") {
            return std::make_unique<MidLeftProjector>(args...) ;
        }
        if(fmt_name == "midright") {
            return std::make_unique<MidRightProjector>(args...) ;
        }
        if(fmt_name == "lowerleft") {
            return std::make_unique<LowerLeftProjector>(args...) ;
        }
        if(fmt_name == "lowermid") {
            return std::make_unique<LowerMidProjector>(args...) ;
        }
        if(fmt_name == "lowerright") {
            return std::make_unique<LowerRightProjector>(args...) ;
        }
        return std::make_unique<CenterProjector>(args...) ;
    }
}


namespace vind
{
    namespace opt
    {
        using Projection = std::function<util::Point2D(LONG, LONG)> ;
        struct VCmdLine::Impl {
            util::DisplayTextPainter dtp_{25, FW_MEDIUM, "Consolas"} ;

            std::unique_ptr<Projector> projector_{nullptr} ;
            int extra_ = 0 ;

            ProjectionMethod proj_method_ = ProjectionMethod::PRIMARY ;

            std::vector<util::Point2D> coords_{} ;
            std::size_t fixed_monitor_idx_ = 0 ;

            std::chrono::seconds fadeout_time_{} ;

            std::chrono::seconds coords_update_interval_{} ;
            std::chrono::system_clock::time_point last_coords_update_{} ;
        } ;

        Message VCmdLine::msg_ ;

        VCmdLine::VCmdLine()
        : OptionCreator("vcmdline"),
          pimpl(std::make_unique<Impl>())
        {}
        VCmdLine::~VCmdLine() noexcept {
            try {
                reset() ;
            }
            catch(const std::exception& UNUSED(e)) {
                PRINT_ERROR("Failed to reset the drawing pixels of the virtual command line.") ;
            }
        }
        VCmdLine::VCmdLine(VCmdLine&&)            = default ;
        VCmdLine& VCmdLine::operator=(VCmdLine&&) = default ;


        void VCmdLine::do_enable() {
            auto& settable = core::SetTable::get_instance() ;

            pimpl->dtp_.set_font(
                    settable.get("cmd_fontsize").get<long>(),
                    settable.get("cmd_fontweight").get<long>(),
                    settable.get("cmd_fontname").get<std::string>()) ;

            pimpl->dtp_.set_text_color(
                    settable.get("cmd_fontcolor").get<std::string>()) ;

            pimpl->dtp_.set_back_color(
                    settable.get("cmd_bgcolor").get<std::string>()) ;

            auto pos = settable.get("cmd_roughpos").get<std::string>() ;
            auto xma = settable.get("cmd_xmargin").get<int>() ;
            auto yma = settable.get("cmd_ymargin").get<int>() ;

            using namespace std::chrono ;
            auto mode = util::A2a(settable.get("cmd_monitor").get<std::string>()) ;
            if(mode == "primary") {
                pimpl->proj_method_ = ProjectionMethod::PRIMARY ;
                pimpl->coords_update_interval_ = 30s ;
                pimpl->fixed_monitor_idx_ = 0 ;
            }
            else if(mode == "active") {
                pimpl->proj_method_ = ProjectionMethod::ACTIVE ;
                pimpl->coords_update_interval_ = 1s ;
                pimpl->fixed_monitor_idx_ = 0 ;
            }
            else if(mode == "all") {
                pimpl->proj_method_ = ProjectionMethod::ALL ;
                pimpl->coords_update_interval_ = 30s ;
                pimpl->fixed_monitor_idx_ = 0 ;
            }
            else {
                pimpl->proj_method_ = ProjectionMethod::FIXED ;
                pimpl->coords_update_interval_ = 30s ;
                pimpl->fixed_monitor_idx_ = util::extract_num(mode) ;
            }

            auto rough_key = settable.get("cmd_roughpos").get<std::string>() ;
            pimpl->projector_ = create_projector(rough_key, xma, yma) ;

            pimpl->extra_ = settable.get("cmd_fontextra").get<int>() ;
            pimpl->fadeout_time_ = std::chrono::seconds(
                    settable.get("cmd_fadeout").get<int>()) ;

            update_drawing_coordinates() ;
        }

        void VCmdLine::do_disable() {
            reset() ;
        }

        void VCmdLine::refresh() {
            util::refresh_display(NULL) ;
        }

        void VCmdLine::clear() {
            msg_.clear() ;
        }

        void VCmdLine::reset() {
            if(!msg_.empty()) {
                clear() ;
                refresh() ;
            }
        }

        void VCmdLine::update_drawing_coordinates() {
            std::vector<util::Box2D> monitors{} ;
            switch(pimpl->proj_method_) {
                case ProjectionMethod::PRIMARY: {
                    monitors.push_back(util::get_primary_metrics()) ;
                    break ;
                }
                case ProjectionMethod::ACTIVE: {
                    util::Point2D active_pos ;
                    if(auto hwnd = util::get_foreground_window()) {
                        auto fg_rect = util::get_window_rect(hwnd) ;
                        active_pos = fg_rect.center() ;
                    }
                    else {
                        active_pos = util::get_cursor_pos() ;
                    }

                    util::MonitorInfo minfo ;
                    util::get_monitor_metrics(active_pos, minfo) ;
                    monitors.push_back(std::move(minfo.rect)) ;
                    break ;
                }
                case ProjectionMethod::ALL: {
                    auto minfo_list = util::get_all_monitor_metrics() ;
                    for(const auto& minfo : minfo_list) {
                        monitors.push_back(std::move(minfo.rect)) ;
                    }
                    break ;
                }
                case ProjectionMethod::FIXED: {
                    auto minfo_list = util::get_all_monitor_metrics() ;

                    if(pimpl->fixed_monitor_idx_ >= minfo_list.size()) {
                        std::stringstream ss ;
                        ss << "The specified monitor number " ;
                        ss << pimpl->fixed_monitor_idx_ << " is out of range." ;
                        PRINT_ERROR(ss.str()) ;
                        monitors.push_back(std::move(minfo_list.front().rect)) ;
                    }
                    else {
                        monitors.push_back(std::move(minfo_list[pimpl->fixed_monitor_idx_].rect)) ;
                    }
                    break ;
                }
            }

            pimpl->coords_.clear() ;
            for(const auto& rect : monitors) {
                auto [x, y] = pimpl->projector_->project(
                        rect.left(), rect.top(),
                        rect.width(), rect.height()) ;

                pimpl->coords_.emplace_back(x, y) ;
            }

            pimpl->last_coords_update_ = std::chrono::system_clock::now() ;
        }

        void VCmdLine::do_process() {
            using namespace std::chrono ;

            if(msg_.empty()) {
                return ;
            }
            if(msg_.fadeoutable()) {
                if(msg_.lifetime() > pimpl->fadeout_time_) {
                    reset() ;
                    return ;
                }
            }

            auto elapsed_time = system_clock::now() - pimpl->last_coords_update_ ;
            if(elapsed_time > pimpl->coords_update_interval_) {
                // The coordinates cache is expired.
                update_drawing_coordinates() ;
            }

            for(const auto& p : pimpl->coords_) {
                pimpl->dtp_.draw(msg_, p.x(), p.y(), pimpl->extra_) ;
            }
        }
    }
}
