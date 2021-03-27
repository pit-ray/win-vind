#include "jump_keybrd.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "i_params.hpp"
#include "io/keybrd.hpp"
#include "io/screen_metrics.hpp"
#include "key/key_absorber.hpp"
#include "key/key_log.hpp"
#include "key/keycodecvt.hpp"
#include "err_logger.hpp"
#include "path.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "entry.hpp"

namespace
{
    //Jump2Any
    float g_max_keybrd_xposs = 0 ;
    float g_max_keybrd_yposs = 0 ;

    using key_pos_t = std::array<float, 256> ;
    key_pos_t g_xposs{} ;
    key_pos_t g_yposs{} ;
}

namespace vind
{
    void Jump2Any::load_config() {
        //initilize
        g_max_keybrd_xposs = 0 ;
        g_max_keybrd_yposs = 0 ;

        g_xposs.fill(0) ;
        g_yposs.fill(0) ;
        const auto filename = path::KEYBRD_MAP() ;

        std::ifstream ifs(path::to_u8path(filename), std::ios::in) ;
        std::string buf ;
        int lnum = 0 ;

        auto ep = [&lnum, &buf, &filename](auto msg) {
            PRINT_ERROR(buf + msg + "\"" + filename + "\", L" + std::to_string(lnum) + ".") ;
        } ;

        while(getline(ifs, buf)) {
            try {
                lnum ++ ;

                if(buf.empty()) {
                    continue ;
                }

                //if top character is #, this line is assumed comment-out.
                if(buf.front() == '#') {
                    continue ;
                }

                const auto vec = util::split(buf, " ") ;

                if(vec.size() != 3) {
                    ep(" is bad syntax in ") ;
                    continue ;
                }

                const auto x = std::stof(vec[0]) ;
                const auto y = stof(vec[1]) ;

                if(x > g_max_keybrd_xposs) g_max_keybrd_xposs = x ;
                if(y > g_max_keybrd_yposs) g_max_keybrd_yposs = y ;

                //specific code
                auto code = vec[2] ;
                //is ascii code
                if(code.size() == 1) {
                    if(const auto vkc = keycodecvt::get_vkc(code.front())) {
                        //overwrite
                        g_xposs[vkc] = x ;
                        g_yposs[vkc] = y ;
                        continue ;
                    }
                    ep(" is not supported in ") ;
                    continue ;
                }

                code = util::A2a(code) ;
                if(code.front() != '<' && code.back() != '>') {
                    ep(" is bad syntax in ") ;
                }

                code = code.substr(1, code.length() - 2) ;
                if(code == "space") {
                    auto&& vkc = keycodecvt::get_vkc(' ') ;
                    g_xposs[vkc] = x ;
                    g_yposs[vkc] = y ;
                    continue ;
                }

                if(auto vkc = keycodecvt::get_sys_vkc(code)) {
                    g_xposs[vkc] = x ;
                    g_yposs[vkc] = y ;
                    continue ;
                }

                ep(" is invalid system key code in ") ;
            }
            catch(const std::runtime_error& e) {
                PRINT_ERROR(e.what()) ;
                continue ;
            }
        }
    }

    const std::string Jump2Any::sname() noexcept {
        return "jump_to_any" ;
    }

    void Jump2Any::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        //reset key state (binded key)
        
        keyabsorber::InstantKeyAbsorber ika ;

        //ignore toggle keys (for example, CapsLock, NumLock, IME....)
        const auto toggle_keys = keyabsorber::get_pressed_list() ;

        RECT rect ;
        screenmetrics::get_conbined_metrics(&rect) ;

        const auto width  = screenmetrics::width(rect) ;
        const auto height = screenmetrics::height(rect) ;

        while(vind::update_background()) {
            if(keyabsorber::is_pressed(KEYCODE_ESC)) return ;

            const auto log = keyabsorber::get_pressed_list() - toggle_keys ;
            if(log.empty()) continue ;

            try {
                for(const auto& vkc : log) {
                    if(keycodecvt::is_unreal_key(vkc))
                        continue ;

                    auto x_pos = static_cast<int>( \
                            g_xposs[vkc] / g_max_keybrd_xposs * width) ;
                    auto y_pos = static_cast<int>( \
                            g_yposs[vkc] / g_max_keybrd_yposs * height) ;

                    if(x_pos == width) 
                        x_pos -= iparams::get_i("screen_pos_buf") ;

                    if(y_pos == height) 
                        y_pos -= iparams::get_i("screen_pos_buf") ;

                    SetCursorPos(x_pos, y_pos) ;

                    for(const auto& key : log) {
                        keybrd::release_keystate(key) ;
                    }
                    return ;
                }
            }
            catch(const std::out_of_range&) {
                continue ;
            }
        }
    }
}
