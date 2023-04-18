#include "gridmove.hpp"

#include "util/color.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/display_text_painter.hpp"
#include "util/hint.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "core/background.hpp"
#include "core/cmdmatcher.hpp"
#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/keycode.hpp"
#include "core/keycodedef.hpp"
#include "core/settable.hpp"

#include "opt/optionlist.hpp"
#include "opt/vcmdline.hpp"

#include <atomic>
#include <chrono>
#include <future>
#include <string>
#include <vector>


namespace
{
    using namespace vind ;

    class HintRenderer {
    private:
        util::DisplayTextPainter painter_ ;
        std::atomic_bool running_ ;

        void draw(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                const std::vector<std::size_t>& start_indices) {
            while(running_) {
                for(int i = 0 ; i < points.size() ; i ++) {
                    if(start_indices[i] == std::string::npos) {
                        continue ;
                    }
                    auto text = " " + hint_texts[i].substr(start_indices[i]) + " " ;
                    painter_.draw(text, points[i], 1) ;
                }
                painter_.refresh() ;

                Sleep(5) ;
            }
        }

    public:
        explicit HintRenderer()
        : painter_(0, 0, ""),
          running_(false)
        {}

        auto launch_drawing(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                const std::vector<std::size_t>& start_indices) {

            running_ = true ;

            auto ft = std::async(
                std::launch::async,
                &HintRenderer::draw,
                this,
                std::cref(points),
                std::cref(hint_texts),
                std::cref(start_indices)) ;
            return ft.share() ;
        }

        void stop_drawing() {
            running_ = false ;
        }

        void reconstruct() {
            auto& settable = core::SetTable::get_instance() ;

            //Colors
            auto [bk_r, bk_g, bk_b] = util::hex2rgb(
                settable.get("gridmove_bgcolor").get<std::string>()) ;
            auto bkcolor = RGB(bk_r, bk_g, bk_b) ;

            auto [tx_r, tx_g, tx_b] = util::hex2rgb(
                settable.get("gridmove_fontcolor").get<std::string>()) ;
            auto txcolor = RGB(tx_r, tx_g, tx_b) ;

            painter_.set_font(
                settable.get("gridmove_fontsize").get<long>(),
                settable.get("gridmove_fontweight").get<long>(),
                settable.get("gridmove_fontname").get<std::string>()) ;

            painter_.set_back_color(bkcolor) ;
            painter_.set_text_color(txcolor) ;
        }
    } ;
}


namespace vind
{
    namespace bind
    {
        struct GridMove::Impl {
            core::Background bg_ {
                opt::ref_global_options_bynames(
                    opt::VCmdLine().name())
            } ;

            HintRenderer hint_renderer_{} ;

            int l1_grid_w_ = 0 ;
            int l1_grid_h_ = 0 ;

            std::vector<util::Point2D> l1_points_ ;
            std::vector<std::string> l1_hint_texts_ ;

            std::vector<core::CmdMatcher> l1_matchers_{} ;

            void assign_l1_hints(
                std::vector<util::Point2D>& points,
                std::vector<util::Hint>& hints,
                std::vector<std::string>& hint_texts) ;
        } ;

        GridMove::GridMove()
        : BindedFuncVoid("gridmove"),
          pimpl(std::make_unique<Impl>())
        {}

        GridMove::~GridMove() noexcept = default ;
        GridMove::GridMove(GridMove&&) = default ;
        GridMove& GridMove::operator=(GridMove&&) = default ;

        void GridMove::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& ihub = core::InputHub::get_instance() ;

            core::InstantKeyAbsorber ika ;

            for(auto& mt : pimpl->l1_matchers_) {
                mt.reset_state() ;
            }

            std::vector<std::size_t> start_indices(pimpl->l1_hint_texts_.size(), 0) ;
            auto ft = pimpl->hint_renderer_.launch_drawing(
                pimpl->l1_points_, pimpl->l1_hint_texts_, start_indices) ;

            while(true) {
                pimpl->bg_.update() ;

                core::CmdUnit::SPtr inputs ;
                std::uint16_t count ;
                if(!ihub.fetch_input(inputs, count, core::get_global_mode(), false)) {
                    continue ;
                }
                if(inputs->is_containing(KEYCODE_ESC)) {
                    break ;
                }

                if(inputs->is_containing(KEYCODE_BKSPACE)) {
                    for(std::size_t i = 0 ; i < pimpl->l1_matchers_.size() ; i ++) {
                        auto& mt = pimpl->l1_matchers_[i] ;
                        mt.backward_state(1) ;

                        if(mt.is_rejected()) {
                            start_indices[i] = std::string::npos ;
                        }
                        else {
                            start_indices[i] = mt.history_size() ;
                        }
                    }

                    util::refresh_display(NULL) ;
                    continue ;
                }

                // Fetch out only the characters.
                core::CmdUnitSet ascii_set{} ;
                for(auto& key : *inputs) {
                    if(!key.is_major_system()) {
                        ascii_set.insert(key) ;
                    }
                }
                if(ascii_set.empty()) {
                    continue ;
                }
                core::CmdUnit ascii_unit(std::move(ascii_set)) ;

                bool all_rejected = true ;
                for(std::size_t i = 0 ; i < pimpl->l1_matchers_.size() ; i ++) {
                    auto& mt = pimpl->l1_matchers_[i] ;
                    mt.update_state(ascii_unit) ;
                    if(mt.is_accepted()) {
                        util::set_cursor_pos(pimpl->l1_points_[i]) ;
                        break ;
                    }

                    if(mt.is_rejected()) {
                        start_indices[i] = std::string::npos ;
                    }
                    else {
                        all_rejected = false ;
                        start_indices[i] = mt.history_size() ;
                    }
                }

                if(all_rejected) {
                    break ;
                }
                else {
                    util::refresh_display(NULL) ;
                }
            }

            pimpl->hint_renderer_.stop_drawing() ;
            using namespace std::chrono ;
            while(ft.wait_for(50ms) == std::future_status::timeout) {}

            util::refresh_display(NULL) ;
        }

        void GridMove::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            auto l1_size = util::split(
                settable.get("gridmove_l1size").get<std::string>(), "x") ;
            pimpl->l1_grid_w_ = std::stoi(l1_size[0]) ;
            pimpl->l1_grid_h_ = std::stoi(l1_size[1]) ;

            std::cout << "L1 Grid Size: " << pimpl->l1_grid_w_ << " x " << pimpl->l1_grid_h_ << std::endl ;

            pimpl->hint_renderer_.reconstruct() ;

            pimpl->l1_points_.clear() ;
            pimpl->l1_hint_texts_.clear() ;
            std::vector<util::Hint> l1_hints ;
            pimpl->assign_l1_hints(pimpl->l1_points_, l1_hints, pimpl->l1_hint_texts_) ;

            pimpl->l1_matchers_.clear() ;
            for(const auto& hint : l1_hints) {
                std::vector<core::CmdUnit::SPtr> cmds ;
                for(const auto& unit : hint) {
                    cmds.push_back(std::make_shared<core::CmdUnit>(unit)) ;
                }
                pimpl->l1_matchers_.emplace_back(std::move(cmds)) ;
            }
        }

        void GridMove::Impl::assign_l1_hints(
                std::vector<util::Point2D>& points,
                std::vector<util::Hint>& hints,
                std::vector<std::string>& hint_texts) {
            auto monitors = util::get_all_monitor_metrics() ;

            std::vector<util::Hint> m_hints ;
            std::vector<std::string> m_hint_texts ;
            util::assign_identifier_hints(monitors.size(), m_hints) ;
            util::convert_hints_to_strings(m_hints, m_hint_texts) ;

            std::vector<util::Hint> c_hints ;
            std::vector<std::string> c_hint_texts ;
            util::assign_identifier_hints(l1_grid_h_ * l1_grid_w_, c_hints) ;
            util::convert_hints_to_strings(c_hints, c_hint_texts) ;

            for(int mi = 0 ; mi < monitors.size() ; mi ++) {
                const auto& rect = monitors[mi].rect ;
                auto cell_w = rect.width() / l1_grid_w_ ;
                auto cell_h = rect.height() / l1_grid_h_ ;

                // the center of the cell at (0, 0).
                auto base_x = rect.left() + cell_w / 2 ;
                auto base_y = rect.top() + cell_h / 2 ;

                for(int ci = 0 ; ci < (l1_grid_h_ * l1_grid_w_) ; ci ++) {
                    points.emplace_back(
                        base_x + (ci % l1_grid_w_) * cell_w,
                        base_y + (ci / l1_grid_w_) * cell_h) ;

                    hint_texts.push_back(m_hint_texts[mi] + c_hint_texts[ci]) ;

                    auto hint = m_hints[mi] ;
                    hint.insert(hint.end(), c_hints[ci].begin(), c_hints[ci].end()) ;
                    hints.push_back(hint) ;
                }
            }
        }
    }
}
