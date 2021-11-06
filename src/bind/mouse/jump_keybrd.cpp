#include "jump_keybrd.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "core/entry.hpp"
#include "core/err_logger.hpp"
#include "core/g_params.hpp"
#include "core/key_absorber.hpp"
#include "core/key_log.hpp"
#include "core/keybrd_layout.hpp"
#include "core/keycodecvt.hpp"
#include "core/ntype_logger.hpp"
#include "core/path.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{

    struct JumpWithKeybrdLayout::Impl {
        float max_keybrd_xposs = 0 ;
        float max_keybrd_yposs = 0 ;

        using KeyPos = std::array<float, 256> ;
        KeyPos xposs{} ;
        KeyPos yposs{} ;
    } ;

    JumpWithKeybrdLayout::JumpWithKeybrdLayout()
    : BindedFuncCreator("jump_cursor_with_keybrd_layout"),
      pimpl(std::make_unique<Impl>())
    {}
    JumpWithKeybrdLayout::~JumpWithKeybrdLayout() noexcept            = default ;
    JumpWithKeybrdLayout::JumpWithKeybrdLayout(JumpWithKeybrdLayout&&)            = default ;
    JumpWithKeybrdLayout& JumpWithKeybrdLayout::operator=(JumpWithKeybrdLayout&&) = default ;

    void JumpWithKeybrdLayout::sprocess() const {
        //reset key state (binded key)
        keyabsorber::InstantKeyAbsorber ika ;

        //ignore toggle keys (for example, CapsLock, NumLock, IME....)
        auto toggle_keys = keyabsorber::get_pressed_list() ;

        auto box = screenmetrics::get_conbined_metrics() ;

        auto width  = box.width() ;
        auto height = box.height() ;

        while(vind::update_background()) {
            if(keyabsorber::is_pressed(KEYCODE_ESC)) return ;

            auto log = keyabsorber::get_pressed_list() - toggle_keys ;
            if(log.empty()) continue ;

            try {
                for(auto& keycode : log) {
                    if(keycodecvt::is_unreal_key(keycode))
                        continue ;

                    auto x_pos = static_cast<int>( \
                            pimpl->xposs[keycode] / pimpl->max_keybrd_xposs * width) ;
                    auto y_pos = static_cast<int>( \
                            pimpl->yposs[keycode] / pimpl->max_keybrd_yposs * height) ;

                    if(x_pos == width) 
                        x_pos -= gparams::get_i("jump_margin") ;

                    if(y_pos == height) 
                        y_pos -= gparams::get_i("jump_margin") ;

                    SetCursorPos(x_pos, y_pos) ;

                    for(auto& key : log) {
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
    void JumpWithKeybrdLayout::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void JumpWithKeybrdLayout::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    void JumpWithKeybrdLayout::reconstruct() {
        auto layoutfile = gparams::get_s("keybrd_layout") ;
        std::filesystem::path filepath ;
        if(!layoutfile.empty()) {
            filepath = path::CONFIG_PATH() / layoutfile ;
        }
        else {
            auto locale_id = GetKeyboardLayout(0) ;
            auto lang_id = static_cast<LANGID>(reinterpret_cast<std::size_t>(locale_id) & 0xffff) ;
            filepath = keybrd_layout::get_layout_filepath(lang_id) ;
        }

        if(filepath.empty()) {
            throw RUNTIME_EXCEPT("The file path of keyboard layout is empty.") ;
        }

        pimpl->max_keybrd_xposs = 0 ;
        pimpl->max_keybrd_yposs = 0 ;

        pimpl->xposs.fill(0) ;
        pimpl->yposs.fill(0) ;

        std::ifstream ifs(filepath, std::ios::in) ;
        if(!ifs.is_open()) {
            throw RUNTIME_EXCEPT("Could not open \"" + filepath.u8string() + "\"") ;
        }
        std::string buf ;
        int lnum = 0 ;

        auto ep = [&lnum, &buf, &filepath](auto msg) {
            PRINT_ERROR(buf + msg + "\"" + filepath.u8string() + "\", L" + std::to_string(lnum) + ".") ;
        } ;

        while(getline(ifs, buf)) {
            try {
                lnum ++ ;

                if(buf.empty()) {
                    continue ;
                }

                //if top character is #, this line is regarded as comment-out.
                if(buf.front() == '#') {
                    continue ;
                }

                auto vec = util::split(buf, " ") ;

                if(vec.size() != 3) {
                    ep(" is bad syntax in ") ;
                    continue ;
                }

                auto x = std::stof(vec[0]) ;
                auto y = stof(vec[1]) ;

                if(x > pimpl->max_keybrd_xposs) pimpl->max_keybrd_xposs = x ;
                if(y > pimpl->max_keybrd_yposs) pimpl->max_keybrd_yposs = y ;

                //specific code
                auto code = vec[2] ;
                //is ascii code
                if(code.size() == 1) {
                    if(auto keycode = keycodecvt::get_keycode(code.front())) {
                        //overwrite
                        pimpl->xposs[keycode] = x ;
                        pimpl->yposs[keycode] = y ;
                        continue ;
                    }
                    ep(" is not supported in ") ;
                    continue ;
                }

                code = util::A2a(code) ;
                if(code.front() != '<' && code.back() != '>') {
                    ep(" is invalid syntax in ") ;
                }

                code = code.substr(1, code.length() - 2) ;
                if(code == "space") {
                    auto&& keycode = keycodecvt::get_keycode(' ') ;
                    pimpl->xposs[keycode] = x ;
                    pimpl->yposs[keycode] = y ;
                    continue ;
                }

                if(auto keycode = keycodecvt::get_sys_keycode(code)) {
                    pimpl->xposs[keycode] = x ;
                    pimpl->yposs[keycode] = y ;
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

}
