#include "display_hinter.hpp"

#include <mutex>
#include <string>

#include "core/char_logger.hpp"
#include "core/settable.hpp"
#include "util/color.hpp"
#include "util/debug.hpp"
#include "util/display_text_painter.hpp"


namespace
{
    template <typename T>
    inline auto add_margin(T&& str) {
        return " " + str + " " ;
    }
}


namespace vind
{
    namespace bind
    {
        struct DisplayHinter::Impl {
            util::DisplayTextPainter painter_ ;
            util::DisplayTextPainter weak_painter_ ;
            long fontsize_ ;

            Impl()
            : painter_(0, 0, ""),
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
            auto& settable = core::SetTable::get_instance() ;

            //Colors
            auto [bk_r, bk_g, bk_b] = util::hex2rgb(
                    settable.get("easyclick_bgcolor").get<std::string>()) ;
            auto bkcolor = RGB(bk_r, bk_g, bk_b) ;

            auto [tx_r, tx_g, tx_b] = util::hex2rgb(
                    settable.get("easyclick_fontcolor").get<std::string>()) ;
            auto txcolor = RGB(tx_r, tx_g, tx_b) ;

            unsigned char decay = settable.get("easyclick_colordecay").get<unsigned char>() ;
            using util::to_gray ;
            char sign = to_gray(tx_r, tx_g, tx_b) > to_gray(bk_r, bk_g, bk_b) ? -1 : 1 ;

            auto txcolor_ready = RGB(
                    tx_r < decay ? 0 : tx_r + sign*decay,
                    tx_g < decay ? 0 : tx_g + sign*decay,
                    tx_b < decay ? 0 : tx_b + sign*decay) ;

            pimpl->fontsize_ = settable.get("easyclick_fontsize").get<long>() ;

            pimpl->painter_.set_font(
                    pimpl->fontsize_,
                    settable.get("easyclick_fontweight").get<long>(),
                    settable.get("easyclick_fontname").get<std::string>()) ;

            pimpl->painter_.set_back_color(bkcolor) ;
            pimpl->painter_.set_text_color(txcolor) ;

            pimpl->weak_painter_ = pimpl->painter_ ;
            pimpl->weak_painter_.set_text_color(txcolor_ready) ;
        }


        void DisplayHinter::paint_all_hints(
                const std::vector<util::Point2D>& positions,
                const std::vector<std::string>& strhints) {

            if(positions.size() != strhints.size()) {
                return ;
            }

            for(std::size_t i = 0 ; i < strhints.size() ; i ++) {
                pimpl->painter_.draw(add_margin(strhints[i]),
                        pimpl->align(positions[i].x()), pimpl->align(positions[i].y()), 1) ;
            }

            pimpl->painter_.refresh() ;
            pimpl->weak_painter_.refresh() ;
        }


        void DisplayHinter::paint_matching_hints(
                const std::vector<util::Point2D>& positions,
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
}
