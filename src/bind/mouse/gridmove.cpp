#include "gridmove.hpp"

#include "util/color.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/display_text_painter.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

#include "core/settable.hpp"


namespace vind
{
    namespace bind
    {
        struct GridMove::Impl {
            int l1_grid_w_ ;
            int l1_grid_h_ ;

            util::DisplayTextPainter painter_ ;
            util::DisplayTextPainter weak_painter_ ;

            Impl()
            : l1_grid_w_(0),
              l1_grid_h_(0),
              painter_(0, 0, ""),
              weak_painter_(painter_)
            {}
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

            auto monitors = util::get_all_monitor_metrics() ;
            for(const auto& m : monitors) {
                const auto& rect = m.rect ;
                auto cell_w = rect.width() / pimpl->l1_grid_w_ ;
                auto cell_h = rect.height() / pimpl->l1_grid_h_ ;

                for(int j = 1 ; j <= pimpl->l1_grid_h_ ; j ++) {
                    for(int i = 1 ; i <= pimpl->l1_grid_w_ ; i ++) {
                        util::Point2D pos{
                            rect.left() + i * cell_w,
                            rect.top() + j * cell_h
                        } ;

                        pimpl->painter_.draw("AA", pos, 1) ;
                    }
                }
            }

            pimpl->painter_.refresh() ;
        }

        void GridMove::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            auto l1_size = util::split(
                settable.get("gridmove_l1size").get<std::string>(), "x") ;
            pimpl->l1_grid_w_ = std::stoi(l1_size[0]) ;
            pimpl->l1_grid_h_ = std::stoi(l1_size[1]) ;

            std::cout << "L1 Grid Size: " << pimpl->l1_grid_w_ << " x " << pimpl->l1_grid_h_ << std::endl ;

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

            pimpl->painter_.set_font(
                settable.get("gridmove_fontsize").get<long>(),
                settable.get("gridmove_fontweight").get<long>(),
                settable.get("gridmove_fontname").get<std::string>()) ;

            pimpl->painter_.set_back_color(bkcolor) ;
            pimpl->painter_.set_text_color(txcolor) ;

            pimpl->weak_painter_ = pimpl->painter_ ;
            pimpl->weak_painter_.set_text_color(txcolor_ready) ;
        }
    }
}
