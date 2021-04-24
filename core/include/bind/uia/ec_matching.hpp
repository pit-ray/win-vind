#ifndef _EC_MATCHING_HPP
#define _EC_MATCHING_HPP

#include <windows.h>

#include <mutex>
#include <vector>

#include "bind/uia/ec_hints.hpp"
#include "bind/uia/point_2d.hpp"
#include "key/keycode_def.hpp"

namespace vind
{
    namespace easyclick {
        void loop_for_key_matching(
                HWND hwnd,
                const std::vector<Point2D>& points,
                const std::vector<Hint>& hints,
                std::vector<unsigned char>& matching_nums,
                std::size_t& need_draw_count,
                std::mutex& mtx,
                const bool& continue_running,
                const unsigned char sendkey=KEYCODE_UNDEFINED) ;
    }
}

#endif
