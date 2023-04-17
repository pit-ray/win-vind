#include "gridmove.hpp"

#include "util/color.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/display_text_painter.hpp"
#include "util/hint.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

#include "core/cmdunit.hpp"
#include "core/inputhub.hpp"
#include "core/keycode.hpp"
#include "core/keycodedef.hpp"
#include "core/settable.hpp"

#include <vector>


namespace
{
    using namespace vind ;

    class HintDrawer {
    private:
        util::DisplayTextPainter painter_ ;
        util::DisplayTextPainter weak_painter_ ;

    public:
        explicit HintDrawer()
        : 
          painter_(0, 0, ""),
          weak_painter_(painter_)
        {}

        void draw(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts) {
            for(int i = 0 ; i < points.size() ; i ++) {
                painter_.draw(" " + hint_texts[i] + " ", points[i], 1) ;
            }
            painter_.refresh() ;
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

            unsigned char decay = settable.get("gridmove_colordecay").get<unsigned char>() ;
            using util::to_gray ;
            char sign = to_gray(tx_r, tx_g, tx_b) > to_gray(bk_r, bk_g, bk_b) ? -1 : 1 ;

            auto txcolor_ready = RGB(
                (tx_r < decay) ? 0 : (tx_r + sign * decay),
                (tx_g < decay) ? 0 : (tx_g + sign * decay),
                (tx_b < decay) ? 0 : (tx_b + sign * decay)) ;

            painter_.set_font(
                settable.get("gridmove_fontsize").get<long>(),
                settable.get("gridmove_fontweight").get<long>(),
                settable.get("gridmove_fontname").get<std::string>()) ;

            painter_.set_back_color(bkcolor) ;
            painter_.set_text_color(txcolor) ;

            weak_painter_ = painter_ ;
            weak_painter_.set_text_color(txcolor_ready) ;
        }
    } ;
}


namespace vind
{
    namespace bind
    {
        struct GridMove::Impl {
            HintDrawer hint_drawer_{} ;

            int l1_grid_w_ = 0 ;
            int l1_grid_h_ = 0 ;

            std::vector<util::Point2D> l1_points ;
            std::vector<util::Hint> l1_hints ;
            std::vector<std::string> l1_hint_texts ;

            void assign_l1_hints(
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

                        std::cout << points.back().x() << ", " << points.back().y() << ": " << hint_texts.back() << std::endl ;
                    }
                }
            }
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

            while(true) {
                pimpl->hint_drawer_.draw(pimpl->l1_points, pimpl->l1_hint_texts) ;

                core::CmdUnit::SPtr inputs ;
                std::uint16_t count ;
                if(!ihub.fetch_input(inputs, count, core::get_global_mode(), false)) {
                    continue ;
                }
                if(inputs->is_containing(KEYCODE_ESC)) {
                    break ;
                }
            }
        }

        void GridMove::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            auto l1_size = util::split(
                settable.get("gridmove_l1size").get<std::string>(), "x") ;
            pimpl->l1_grid_w_ = std::stoi(l1_size[0]) ;
            pimpl->l1_grid_h_ = std::stoi(l1_size[1]) ;

            std::cout << "L1 Grid Size: " << pimpl->l1_grid_w_ << " x " << pimpl->l1_grid_h_ << std::endl ;

            pimpl->hint_drawer_.reconstruct() ;

            pimpl->l1_points.clear() ;
            pimpl->l1_hints.clear() ;
            pimpl->l1_hint_texts.clear() ;
            pimpl->assign_l1_hints(pimpl->l1_points, pimpl->l1_hints, pimpl->l1_hint_texts) ;
        }
    }
}
