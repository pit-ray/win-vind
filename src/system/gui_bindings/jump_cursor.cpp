#include "screen_metrics.hpp"
#include "jump_cursor.hpp"

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

#include "key_absorber.hpp"
#include "keybrd_eventer.hpp"
#include "msg_logger.hpp"
#include "i_params.hpp"
#include "key_log.hpp"
#include "vkc_converter.hpp"
#include "utility.hpp"
#include "path.hpp"

using namespace std ;

namespace JumpCursorUtility
{
    static const ScreenMetrics _scmet{} ;
}

using namespace JumpCursorUtility ;

//Jump2Left
const string Jump2Left::sname() noexcept
{
    return "jump_to_left" ;
}

void Jump2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(0, pos.y) ;
}


//Jump2Right
const string Jump2Right::sname() noexcept
{
    return "jump_to_right" ;
}

void Jump2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(_scmet.width() - iParams::get_i("screen_pos_buf"), pos.y) ;
}


//Jump2Top
const string Jump2Top::sname() noexcept
{
    return "jump_to_top" ;
}

void Jump2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, 0) ;
}


//Jump2Bottom
const string Jump2Bottom::sname() noexcept
{
    return "jump_to_bottom" ;
}

void Jump2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, _scmet.height() - iParams::get_i("screen_pos_buf")) ;
}


//Jump2XCenter
const string Jump2XCenter::sname() noexcept
{
    return "jump_to_xcenter" ;
}

void Jump2XCenter::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(_scmet.width() / 2, pos.y) ;
}


//Jump2YCenter
const string Jump2YCenter::sname() noexcept
{
    return "jump_to_ycenter" ;
}

void Jump2YCenter::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, _scmet.height() / 2) ;
}


//Jump2Any
namespace JumpCursorUtility
{
    static float max_keybrd_xposs = 0 ;
    static float max_keybrd_yposs = 0 ;

    using key_pos_t = std::array<float, 256> ;
    static key_pos_t _xposs{} ;
    static key_pos_t _yposs{} ;
    void load_config() {
        //initilize
        max_keybrd_xposs = 0 ;
        max_keybrd_yposs = 0 ;

        _xposs.fill(0) ;
        _yposs.fill(0) ;
        const auto filename = Path::KEYBRD_MAP() ;

        ifstream ifs(filename, ios::in) ;
        string buf ;
        int lnum = 0 ;

        auto ep = [&lnum, &buf, &filename](auto msg) {
            ERROR_PRINT(buf + msg + "\"" + filename + "\", L" + std::to_string(lnum) + ".") ;
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

                const auto vec = Utility::split(buf, " ") ;

                if(vec.size() != 3) {
                    ep(" is bad syntax in ") ;
                    continue ;
                }

                const auto x = stof(vec[0]) ;
                const auto y = stof(vec[1]) ;

                if(x > max_keybrd_xposs) max_keybrd_xposs = x ;
                if(y > max_keybrd_yposs) max_keybrd_yposs = y ;

                //specific code
                auto code = vec[2] ;
                //is ascii code
                if(code.size() == 1) {
                    if(const auto vkc = VKCConverter::get_vkc(code.front())) {
                        //overwrite
                        _xposs[vkc] = x ;
                        _yposs[vkc] = y ;
                        continue ;
                    }
                    ep(" is not supported in ") ;
                    continue ;
                }

                code = Utility::A2a(code) ;
                if(code.front() != '<' && code.back() != '>') {
                    ep(" is bad syntax in ") ;
                }

                code = code.substr(1, code.length() - 2) ;
                if(code == "space") {
                    auto&& vkc = VKCConverter::get_vkc(' ') ;
                    _xposs[vkc] = x ;
                    _yposs[vkc] = y ;
                    continue ;
                }

                if(auto vkc = VKCConverter::get_sys_vkc(code)) {
                    _xposs[vkc] = x ;
                    _yposs[vkc] = y ;
                    continue ;
                }

                ep(" is invalid system key code in ") ;
            }
            catch(const std::runtime_error& e) {
                ERROR_PRINT(e.what()) ;
                continue ;
            }
        }
    }
}

const string Jump2Any::sname() noexcept
{
    return "jump_to_any" ;
}

void Jump2Any::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    //reset key state (binded key)
    
    KeyAbsorber::close_with_refresh() ;

    //ignore toggle keys (for example, CapsLock, NumLock, IME....)
    const auto toggle_keys = KeyAbsorber::get_pressed_list() ;

    while(true) {
        Utility::get_win_message() ;

        if(KeyAbsorber::is_pressed(VKC_ESC)) return ;

        const auto log = KeyAbsorber::get_pressed_list() - toggle_keys ;
        if(log.empty()) continue ;

        try {
            for(const auto& vkc : log) {
                if(VKCConverter::is_unreal_key(vkc))
                    continue ;

                auto x_pos = static_cast<int>( \
                        _xposs[vkc] / max_keybrd_xposs * _scmet.width()) ;
                auto y_pos = static_cast<int>( \
                        _yposs[vkc] / max_keybrd_yposs * _scmet.height()) ;

                if(x_pos == _scmet.width())
                    x_pos -= iParams::get_i("screen_pos_buf") ;

                if(y_pos == _scmet.height()) 
                    y_pos -= iParams::get_i("screen_pos_buf") ;

                SetCursorPos(x_pos, y_pos) ;

                for(const auto& key : log) {
                    KeybrdEventer::release_keystate(key) ;
                }
                return ;
            }
        }
        catch(const out_of_range&) {
            continue ;
        }

        Sleep(5) ;
    }
}


//Jump2ActiveWindow
const string Jump2ActiveWindow::sname() noexcept
{
    return "jump_to_active_window" ;
}

void Jump2ActiveWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    const auto hwnd = GetForegroundWindow() ;
    if(!hwnd) {
        throw RUNTIME_EXCEPT("GetForegoundWindow return nullptr") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("cannot get window rect") ;
    }

    auto&& xpos = static_cast<int>(rect.left + (rect.right - rect.left) / 2) ;
    auto&& ypos = static_cast<int>(rect.top + (rect.bottom - rect.top) / 2) ;

    SetCursorPos(xpos, ypos) ;
}
