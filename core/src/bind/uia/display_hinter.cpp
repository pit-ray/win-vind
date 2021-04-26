#include "bind/uia/display_hinter.hpp"

#include <mutex>
#include <string>

#include "bind/base/char_logger.hpp"
#include "bind/uia/point_2d.hpp"
#include "coreio/i_params.hpp"
#include "io/display_text_painter.hpp"
#include "key/key_absorber.hpp"
#include "util/color.hpp"

namespace
{
    template <typename T>
    inline auto add_margin(T&& str) {
        return " " + str + " " ;
    }
}


namespace vind
{
    struct DisplayHinter::Impl {
        DisplayTextPainter painter_ ;
        DisplayTextPainter weak_painter_ ;
        long fontsize_ ;

        Impl()
        : painter_(
            iparams::get_l("easy_click_font_size"),
            iparams::get_l("easy_click_font_weight"),
            iparams::get_s("easy_click_font_name")),
          weak_painter_(painter_),
          fontsize_(0)
        {}

        // A detected positon is the center one of object.
        // And, TextOutA draws from a left-upper coordinate, so must move.
        template <typename T>
        auto align(T&& v) {
            return v - fontsize_ / 2 ;
        }
    } ;

    DisplayHinter::DisplayHinter()
    : pimpl(std::make_unique<Impl>())
    {}

    DisplayHinter::~DisplayHinter() noexcept                 = default ;
    DisplayHinter::DisplayHinter(DisplayHinter&&)            = default ;
    DisplayHinter& DisplayHinter::operator=(DisplayHinter&&) = default ;

    void DisplayHinter::load_config() {
        //Colors
        auto [bk_r, bk_g, bk_b] = util::hex2rgb(iparams::get_s("easy_click_font_bkcolor")) ;
        auto bkcolor = RGB(bk_r, bk_g, bk_b) ;

        auto [tx_r, tx_g, tx_b] = util::hex2rgb(iparams::get_s("easy_click_font_color")) ;
        auto txcolor = RGB(tx_r, tx_g, tx_b) ;

        unsigned char decay = iparams::get_uc("easy_click_matching_color_decay") ;
        using util::to_gray ;
        char sign = to_gray(tx_r, tx_g, tx_b) > to_gray(bk_r, bk_g, bk_b) ? -1 : 1 ;

        auto txcolor_ready = RGB(
                tx_r < decay ? 0 : tx_r + sign*decay,
                tx_g < decay ? 0 : tx_g + sign*decay,
                tx_b < decay ? 0 : tx_b + sign*decay) ;

        pimpl->fontsize_ = iparams::get_l("easy_click_font_size") ;

        pimpl->painter_.set_font(
                pimpl->fontsize_,
                iparams::get_l("easy_click_font_weight"),
                iparams::get_s("easy_click_font_name")) ;

        pimpl->painter_.set_back_color(bkcolor) ;
        pimpl->painter_.set_text_color(txcolor) ;

        pimpl->weak_painter_ = pimpl->painter_ ;
        pimpl->weak_painter_.set_text_color(txcolor_ready) ;
    }


    void DisplayHinter::paint_all_hints(
            const std::vector<Point2D>& positions,
            const std::vector<std::string>& strhints) {

        if(positions.size() != strhints.size()) {
            return ;
        }

        for(std::size_t i = 0 ; i < strhints.size() ; i ++) {
            pimpl->painter_.draw(add_margin(strhints[i]),
                    pimpl->align(positions[i].x()), pimpl->align(positions[i].y()), 1) ;
        }

        pimpl->painter_.refresh() ;
    }


    void DisplayHinter::paint_matching_hints(
            const std::vector<Point2D>& positions,
            const std::vector<std::string>& strhints,
            const std::vector<unsigned char>& matched_counts) {

        if(positions.size() != strhints.size()) {
            return ;
        }

        for(std::size_t i = 0 ; i < strhints.size() ; i ++) {
            if(matched_counts[i] == 0) {
                continue ;
            }
            pimpl->painter_.draw(add_margin(strhints[i]),
                    pimpl->align(positions[i].x()), pimpl->align(positions[i].y()), 1) ;

            //overdraw with the weak text color.
            pimpl->weak_painter_.draw(" " + strhints[i].substr(0, matched_counts[i]),
                    pimpl->align(positions[i].x()), pimpl->align(positions[i].y()), 1) ;
        }
        pimpl->painter_.refresh() ;
        pimpl->weak_painter_.refresh() ;
    }
}
