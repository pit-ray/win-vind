#include "screen_metrics.hpp"
#include "jump_cursor.hpp"

#include <windows.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <unordered_map>

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

bool Jump2Left::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(0, pos.y) ;
    return true ;
}


//Jump2Right
const string Jump2Right::sname() noexcept
{
    return "jump_to_right" ;
}

bool Jump2Right::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(_scmet.width() - iParams::get_i("screen_pos_buf"), pos.y) ;
    return true ;
}


//Jump2Top
const string Jump2Top::sname() noexcept
{
    return "jump_to_top" ;
}

bool Jump2Top::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, 0) ;
    return true ;
}


//Jump2Bottom
const string Jump2Bottom::sname() noexcept
{
    return "jump_to_bottom" ;
}

bool Jump2Bottom::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, _scmet.height() - iParams::get_i("cursor_pos_buf")) ;
    return true ;
}


//Jump2XCenter
const string Jump2XCenter::sname() noexcept
{
    return "jump_to_xcenter" ;
}

bool Jump2XCenter::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(_scmet.width() / 2, pos.y) ;
    return true ;
}


//Jump2YCenter
const string Jump2YCenter::sname() noexcept
{
    return "jump_to_ycenter" ;
}

bool Jump2YCenter::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    POINT pos ;
    GetCursorPos(&pos) ;
    SetCursorPos(pos.x, _scmet.height() / 2) ;
    return true ;
}


//Jump2Any
namespace JumpCursorUtility
{
    static float max_keybrd_xpos = 0 ;
    static float max_keybrd_ypos = 0 ;

    inline static void write_error(const string& buf, const string& filename) {
        ERROR_STREAM << buf << " is bad syntax in " << filename << ". (JumpCursorUtility::load_config)\n" ;
    }

    using key_pos_t = std::unordered_map<unsigned char, std::pair<float, float>> ;
    static key_pos_t _keypos{} ;
    void load_config() noexcept {
        //initilize
        max_keybrd_xpos = 0 ;
        max_keybrd_ypos = 0 ;

        try {
            _keypos.clear() ;
            const auto filename = Path::KEYBRD_MAP() ;

            ifstream ifs(filename, ios::in) ;
            string buf ;

            while(getline(ifs, buf)) {
                if(buf.empty()) {
                    continue ;
                }

                //if top character is #, this line is assumed comment-out.
                if(buf.front() == '#') {
                    continue ;
                }

                const auto vec = Utility::split(buf, " ") ;

                if(vec.size() != 3) {
                    write_error(buf, filename) ;
                    continue ;
                }

                const auto x = stof(vec[0]) ;
                const auto y = stof(vec[1]) ;

                if(x > max_keybrd_xpos) {
                    max_keybrd_xpos = x ;
                }

                if(y > max_keybrd_ypos) {
                    max_keybrd_ypos = y ;
                }

                //specific code
                if(vec[2] == "Space") {
                    _keypos[VKCConverter::get_vkc(' ')] = make_pair(x, y) ;
                    continue ;
                }

                if(auto vkc = VKCConverter::get_sys_vkc(vec[2])) {
                    _keypos[vkc] = make_pair(x, y) ;
                    continue ;
                }

                //is ascii code
                if(vec[2].size() == 1) {
                    const auto vkc = VKCConverter::get_vkc(vec[2].front()) ;
                    if(vkc != '\0') {
                        //overwrite
                        _keypos[vkc] = make_pair(x, y) ;
                        continue ;
                    }
                }

                write_error(buf, filename) ;
            }
        }

        catch(const exception& e) {
            ERROR_STREAM << "std::fstream: " <<  e.what() << " (JumpCursorUtility::load_config)\n" ;
            return ;
        }
    }
}

const string Jump2Any::sname() noexcept
{
    return "jump_to_any" ;
}

bool Jump2Any::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    //reset key state (binded key)
    for(const auto& key : KeyAbsorber::get_pressed_list()) {
        if(!KeybrdEventer::release_keystate(key)) {
            return false ;
        }
    }

    //ignore locked key (for example, CapsLock, NumLock, Kana....)
    const auto toggle_keys = KeyAbsorber::get_pressed_list() ;

    MSG msg ;
    while(true) {
        //MessageRoop
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;

            DispatchMessage(&msg) ;
        }

        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            return true ;
        }

        const auto log = KeyAbsorber::get_pressed_list() - toggle_keys ;

        if(log.is_empty()) {
            continue ;
        }

        try {
            const auto pos = _keypos.at(*log.cbegin()) ;

            auto x_pos = static_cast<int>(pos.first / max_keybrd_xpos * _scmet.width()) ;
            auto y_pos = static_cast<int>(pos.second / max_keybrd_ypos * _scmet.height()) ;
            if(x_pos == _scmet.width()) {
                x_pos -= iParams::get_i("screen_pos_buf") ;
            }
            if(y_pos == _scmet.height()) {
                y_pos -= iParams::get_i("screen_pos_buf") ;
            }

            SetCursorPos(x_pos, y_pos) ;

            for(const auto& key : log) {
                if(!KeybrdEventer::release_keystate(key)) {
                    ERROR_STREAM << "failed release key (Jump2Any::is_release_keystate)\n" ;
                    return false ;
                }
            }
            return true ;
        }
        catch(const out_of_range&) {
            continue ;
        }

        Sleep(5) ;
    }
    return true ;
}


//Jump2ActiveWindow
const string Jump2ActiveWindow::sname() noexcept
{
    return "jump_to_active_window" ;
}

bool Jump2ActiveWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    const auto hwnd = GetForegroundWindow() ;
    if(!hwnd) {
        WIN_ERROR_STREAM << "GetForegoundWindow return nullptr "\
        << " (bf_jump_cursor.cpp::Jump2ActiveWindow::do_process::GetForegroundWindow)\n" ;
        return false ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        WIN_ERROR_STREAM << " (bf_jump_cursor.cpp::Jump2ActiveWindow::do_process::GetWindowRect)\n" ;
        return false ;
    }

    const auto xpos = static_cast<int>(rect.left + (rect.right - rect.left) / 2) ;
    const auto ypos = static_cast<int>(rect.top + (rect.bottom - rect.top) / 2) ;

    SetCursorPos(xpos, ypos) ;
    return true ;
}