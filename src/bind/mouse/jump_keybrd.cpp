#define NOMINMAX

#include "jump_keybrd.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "core/background.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/keycode.hpp"
#include "core/keylayout.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"
#include "opt/optionlist.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        struct JumpWithKeybrdLayout::Impl {
            using KeyPos = std::array<float, 256> ;
            KeyPos xposs_{} ;
            KeyPos yposs_{} ;

            core::Background bg_{opt::all_global_options()} ;

            Impl()
            : xposs_(),
              yposs_(),
              bg_(opt::search_options(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::VCmdLine().name()
              ))
            {}
        } ;

        JumpWithKeybrdLayout::JumpWithKeybrdLayout()
        : BindedFuncVoid("jump_cursor_with_keybrd_layout"),
          pimpl(std::make_unique<Impl>())
        {}
        JumpWithKeybrdLayout::~JumpWithKeybrdLayout() noexcept            = default ;
        JumpWithKeybrdLayout::JumpWithKeybrdLayout(JumpWithKeybrdLayout&&)            = default ;
        JumpWithKeybrdLayout& JumpWithKeybrdLayout::operator=(JumpWithKeybrdLayout&&) = default ;

        void JumpWithKeybrdLayout::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;

            //reset key state (binded key)
            core::InstantKeyAbsorber ika ;

            //ignore toggle keys (for example, CapsLock, NumLock, IME....)
            auto toggle_keys = igate.pressed_list() ;

            while(true) {
                if(!pimpl->bg_.update()) {
                    continue ;
                }

                core::CmdUnit::SPtr inputs ;
                std::uint16_t count ;
                if(!ihub.fetch_input(
                        inputs, count, core::get_global_mode(), false)) {
                    continue ;
                }

                if(inputs->is_containing(KEYCODE_ESC)) {
                    return ;
                }

                for(const auto& keycode : *inputs) {
                    if(toggle_keys.is_containing(keycode)) {
                        continue ;
                    }
                    if(keycode.is_unreal()) {
                        continue ;
                    }
                    auto x = pimpl->xposs_[keycode.to_code()] ;
                    auto y = pimpl->yposs_[keycode.to_code()] ;

                    util::MonitorInfo minfo ;
                    util::get_monitor_metrics(util::get_cursor_pos(), minfo) ;

                    auto x_base = static_cast<decltype(x)>(minfo.rect.left()) ;
                    auto y_base = static_cast<decltype(y)>(minfo.rect.top()) ;
                    auto width  = static_cast<decltype(x)>(minfo.rect.width()) ;
                    auto height = static_cast<decltype(y)>(minfo.rect.height()) ;

                    x = x_base + x * width ;
                    y = y_base + y * height ;

                    auto& settable = core::SetTable::get_instance() ;
                    auto margin = settable.get("jump_margin").get<int>() ;

                    x = std::max(x, x_base + margin) ;
                    y = std::max(y, y_base + margin) ;
                    x = std::min(x, x_base + width - margin) ;
                    y = std::min(y, y_base + height - margin) ;

                    util::set_cursor_pos(x, y) ;

                    for(const auto& key : *inputs) {
                        igate.release_keystate(key) ;
                    }
                    return ;
                }
            }
        }

        void JumpWithKeybrdLayout::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;
            auto layoutfile = settable.get("keybrd_layout").get<std::string>() ;
            std::filesystem::path filepath ;
            if(!layoutfile.empty()) {
                filepath = core::RESOURCE_ROOT_PATH() / layoutfile ;
            }
            else {
                auto locale_id = GetKeyboardLayout(0) ;
                auto lang_id = static_cast<LANGID>(reinterpret_cast<std::size_t>(locale_id) & 0xffff) ;
                filepath = core::get_keylayout_filepath(lang_id) ;
            }

            if(filepath.empty()) {
                throw RUNTIME_EXCEPT("The file path of keyboard layout is empty.") ;
            }

            pimpl->xposs_.fill(0) ;
            pimpl->yposs_.fill(0) ;

            std::ifstream ifs(filepath, std::ios::in) ;
            if(!ifs.is_open()) {
                throw RUNTIME_EXCEPT("Could not open \"" + filepath.u8string() + "\"") ;
            }
            std::string buf ;
            int lnum = 0 ;

            auto ep = [&lnum, &buf, &filepath](auto msg) {
                PRINT_ERROR(buf + msg + "\"" + filepath.u8string() + "\", L" + std::to_string(lnum) + ".") ;
            } ;

            float max_x = 0.0f ;
            float max_y = 0.0f ;
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

                    if(x > max_x) max_x = x ;
                    if(y > max_y) max_y = y ;

                    //specific code
                    auto code = vec[2] ;
                    //is ascii code
                    if(code.size() == 1) {
                        core::KeyCode keycode(code.front()) ;
                        if(keycode) {
                            //overwrite
                            pimpl->xposs_[keycode.to_code()] = x ;
                            pimpl->yposs_[keycode.to_code()] = y ;
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
                    core::KeyCode keycode(code) ;

                    if(keycode) {
                        pimpl->xposs_[keycode.to_code()] = x ;
                        pimpl->yposs_[keycode.to_code()] = y ;
                        continue ;
                    }

                    ep(" is invalid system key code in ") ;
                }
                catch(const std::runtime_error& e) {
                    PRINT_ERROR(e.what()) ;
                    continue ;
                }
            }

            if(max_x == 0 || max_y == 0) {
                PRINT_ERROR("The keyboard layout is invalid.") ;
            }

            for(std::size_t i = 0 ; i < pimpl->xposs_.size() ; i ++) {
                pimpl->xposs_[i] /= max_x ;
                pimpl->yposs_[i] /= max_y ;
            }
        }
    }
}
