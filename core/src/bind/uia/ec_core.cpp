#include "bind/uia/ec_core.hpp"

#include <windows.h>

#include <future>
#include <stdexcept>

#include "bind/uia/ec_hints.hpp"
#include "bind/uia/ec_matching.hpp"
#include "bind/uia/ec_scan.hpp"
#include "bind/uia/point_2d.hpp"
#include "coreio/i_params.hpp"
#include "io/display_text_painter.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "util/color.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace easyclick {
        void do_easy_click(unsigned char sendkey) {
            auto hwnd = GetForegroundWindow() ;
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not a foreground window.") ;
            }

            std::vector<Point2D> obj_points ;
            obj_points.reserve(2048) ;

            scan_gui_objects(obj_points) ;

            if(!obj_points.empty()) {
                auto hints = assign_identifier_hints(obj_points.size()) ;
                auto hints_str = convert_hints_to_strings(hints) ;

                std::mutex l_mtx ;
                std::vector<unsigned char> matching_nums(hints.size(), 0) ;
                auto need_draw_count = hints.size() ;

                bool continue_running = true ;

                //create new thread for matching inputed keys with hints.
                auto input_ft = std::async(
                        std::launch::async,
                        loop_for_key_matching,
                        hwnd, std::cref(obj_points), std::cref(hints),
                        std::ref(matching_nums), std::ref(need_draw_count),
                        std::ref(l_mtx), std::cref(continue_running), sendkey) ;

                try {
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

                    auto fontsize = iparams::get_l("easy_click_font_size") ;
                    DisplayTextPainter dtp(
                            fontsize,
                            iparams::get_l("easy_click_font_weight"),
                            iparams::get_s("easy_click_font_name")) ;
                    dtp.set_back_color(bkcolor) ;
                    dtp.set_text_color(txcolor) ;

                    auto dtp_weak(dtp) ; //copy construct
                    dtp_weak.set_text_color(txcolor_ready) ;

                    // A detected positon is the center one of object.
                    // And, TextOutA draws from a left-upper coordinate, so must move.
                    auto align = [fontsize] (auto&& v) {
                        return v - fontsize / 2 ;
                    } ;

                    auto add_margin = [](const auto& str) {
                        return " " + str + " " ;
                    } ;

                    using namespace std::chrono ;
                    while(input_ft.wait_for(50ms) == std::future_status::timeout) {
                        std::lock_guard<std::mutex> scoped_lock(l_mtx) ; //atomic ---------- (0)

                        if(need_draw_count == hints.size()) {
                            for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                                dtp.draw(add_margin(hints_str[i]),
                                        align(obj_points[i].x()), align(obj_points[i].y()), 1) ;
                            }
                        }
                        else {
                            for(std::size_t i = 0 ; i < hints_str.size() ; i ++) {
                                if(matching_nums[i] == 0) {
                                    continue ;
                                }
                                dtp.draw(add_margin(hints_str[i]),
                                        align(obj_points[i].x()), align(obj_points[i].y()), 1) ;

                                //overdraw with the weak text color.
                                dtp_weak.draw(" " + hints_str[i].substr(0, matching_nums[i]),
                                        align(obj_points[i].x()), align(obj_points[i].y()), 1) ;
                            }
                        }

                        dtp.refresh() ;
                        dtp_weak.refresh() ;
                        //------------------------------------------------------------------ (0)
                    }
                }
                catch(const std::exception& e) {
                    continue_running = false ;
                    throw e ;
                }

                util::refresh_display(NULL) ; //remove hints in display

                //release all keys
                for(unsigned char key : keyabsorber::get_pressed_list()) {
                    keyabsorber::release_virtually(key) ;
                }
            }
        }
    }
}
