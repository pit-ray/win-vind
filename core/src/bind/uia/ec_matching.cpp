#include "bind/uia/ec_matching.hpp"

#include <windows.h>

#include <mutex>
#include <vector>

#include "bind/base/char_logger.hpp"
#include "bind/uia/ec_hints.hpp"
#include "bind/uia/point_2d.hpp"
#include "entry.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace easyclick {
        // [Return value] count that need to draw
        std::size_t match_with_hints(
                const KeyLoggerBase* const pc_lgr,
                const std::vector<Hint>& hints,
                std::vector<unsigned char>& matching_nums,
                long* p_matched_index=nullptr) {

            if(p_matched_index) {
                *p_matched_index = -1 ;
            }

            if(pc_lgr->empty()) {
                return hints.size() ; //all is matched
            }

            std::size_t draw_count = 0 ;
            for(std::size_t i = 0 ; i < hints.size() ; i ++) {
                std::size_t seq_idx ;
                for(seq_idx = 0 ; seq_idx < pc_lgr->size() ; seq_idx ++) {
                    try {
                        if(!pc_lgr->at(seq_idx).is_containing(hints[i].at(seq_idx))) {
                            break ;
                        }
                    }
                    catch(const std::out_of_range&) {break ;}
                }

                if(seq_idx == pc_lgr->size()) {
                    draw_count ++ ;
                    matching_nums[i] = static_cast<unsigned char>(seq_idx) ;
                }
                else {
                    matching_nums[i] = 0 ;
                }

                if(seq_idx == hints[i].size()) {
                    if(p_matched_index)
                        *p_matched_index = static_cast<long>(i) ;
                    return 1 ;
                }
            }

            return draw_count ;
        }

        void loop_for_key_matching(
                HWND hwnd,
                const std::vector<Point2D>& points,
                const std::vector<Hint>& hints,
                std::vector<unsigned char>& matching_nums,
                std::size_t& need_draw_count,
                std::mutex& mtx,
                const bool& continue_running,
                const unsigned char sendkey) {

            keyabsorber::InstantKeyAbsorber ika ;
            CharLogger lgr{
                KEYCODE_ESC,
                KEYCODE_BKSPACE
            };

            while(vind::update_background() && continue_running) {
                if(!CHAR_LOGGED(lgr.logging_state())) {
                    continue ;
                }

                if(lgr.latest().is_containing(KEYCODE_ESC)) {
                    keyabsorber::release_virtually(KEYCODE_ESC) ;
                    return ;
                }

                if(lgr.latest().is_containing(KEYCODE_BKSPACE)) {
                    keyabsorber::release_virtually(KEYCODE_BKSPACE) ;
                    if(lgr.size() == 1) {
                        return ;
                    }

                    lgr.remove_from_back(2) ;

                    std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic ---------- (1)

                    need_draw_count = match_with_hints(&lgr, hints, matching_nums) ; //update matching list

                    continue ; //------------------------------------------------------ (1)
                }


                std::lock_guard<std::mutex> scoped_lock(mtx) ; // atomic -------------- (2)

                long full_match_idx ;
                need_draw_count = match_with_hints(&lgr, hints, matching_nums, &full_match_idx) ;

                if(full_match_idx >= 0) {
                    SetCursorPos(points[full_match_idx].x(), points[full_match_idx].y()) ;
                    if(sendkey != KEYCODE_UNDEFINED) {
                        mouse::click(sendkey) ;
                    }
                    return ;
                }

                if(need_draw_count == 0)
                    lgr.remove_from_back(1) ;
                else
                    util::refresh_display(hwnd) ;
                //--------------------------------------------------------------------- (2)
            }
        }
    }
}
