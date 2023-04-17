#include "gridmove.hpp"

#include "util/color.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/display_text_painter.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

#include "core/inputhub.hpp"
#include "core/settable.hpp"


namespace
{
    using namespace vind ;

    class TileHinter {
    private:
        int l1_grid_w_ ;
        int l1_grid_h_ ;

        util::DisplayTextPainter painter_ ;
        util::DisplayTextPainter weak_painter_ ;

    public:
        explicit TileHinter()
        : l1_grid_w_(0),
          l1_grid_h_(0),
          painter_(0, 0, ""),
          weak_painter_(painter_)
        {}

        void draw_hints() {
            auto monitors = util::get_all_monitor_metrics() ;
            for(const auto& m : monitors) {
                const auto& rect = m.rect ;
                auto cell_w = rect.width() / l1_grid_w_ ;
                auto cell_h = rect.height() / l1_grid_h_ ;

                // the center of the cell at (0, 0).
                auto base_x = rect.left() + cell_w / 2 ;
                auto base_y = rect.top() + cell_h / 2 ;

                for(int j = 0 ; j < l1_grid_h_ ; j ++) {
                    for(int i = 0 ; i < l1_grid_w_ ; i ++) {
                        util::Point2D pos{
                            base_x + i * cell_w,
                            base_y + j * cell_h
                        } ;

                        painter_.draw("AA", pos, 1) ;
                    }
                }
            }

            painter_.refresh() ;
        }

        void reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            auto l1_size = util::split(
                settable.get("gridmove_l1size").get<std::string>(), "x") ;
            l1_grid_w_ = std::stoi(l1_size[0]) ;
            l1_grid_h_ = std::stoi(l1_size[1]) ;

            std::cout << "L1 Grid Size: " << l1_grid_w_ << " x " << l1_grid_h_ << std::endl ;

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
            TileHinter tile_hinter_{} ;
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
                pimpl->tile_hinter_.draw_hints() ;

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
            pimpl->tile_hinter_.reconstruct() ;
        }
    }
}
