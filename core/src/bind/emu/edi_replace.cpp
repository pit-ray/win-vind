#include "edi_replace.hpp"

#include <windows.h>
#include <iostream>

#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/text_analyzer.hpp"
#include "util/def.hpp"
#include "entry.hpp"

namespace
{
    using namespace vind ;

    inline auto is_shift(const unsigned key) noexcept {
        return key == KEYCODE_SHIFT || key == KEYCODE_LSHIFT || key == KEYCODE_RSHIFT ;
    }

    template <typename FuncT>
    inline void loop_for_keymatching(FuncT&& func) {
        //reset keys downed in order to call this function.
        for(const auto& key : keyabsorber::get_pressed_list()) {
            if(is_shift(key)) continue ;
            keybrd::release_keystate(key) ;
        }

        while(vind::update_background()) {
            if(keyabsorber::is_pressed(KEYCODE_ESC)) {
                return ;
            }
            const auto log = keyabsorber::get_pressed_list() ;

            if(!log.is_containing(KEYCODE_SHIFT)) {
                //not shifted
                for(const auto& key : log) {
                    //For example, if replace by 'i' and 'i' key is downed,
                    //immediately will call "insert-mode", so release 'i'.
                    keybrd::release_keystate(key) ;

                    if(!keycodecvt::get_ascii(key)) {
                        continue ;
                    }
                    if(func(key)) {
                        return ;
                    }
                }
            }
            else {
                //shifted
                for(const auto& key : log) {
                    if(is_shift(key)) continue ;
                    keybrd::release_keystate(key) ;
                    if(!keycodecvt::get_shifted_ascii(key)) {
                        continue ;
                    }
                    if(func(key, true)) {
                        return ;
                    }
                }
            }
        }
    }
}


namespace vind
{
    //EdiNRplaceChar
    const std::string EdiNReplaceChar::sname() noexcept {
        return "edi_n_replace_char" ;
    }
    void EdiNReplaceChar::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        loop_for_keymatching([repeat_num](const auto& keycodes, const bool shifted=false) {

            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_DELETE) ;

                if(shifted) keybrd::pushup(KEYCODE_LSHIFT, keycodes) ;
                else keybrd::pushup(keycodes) ;
            }

            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                keybrd::pushup(KEYCODE_LEFT) ;

            return true ; //terminate looping
        }) ;
    }


    //EdiNReplaceSequence
    const std::string EdiNReplaceSequence::sname() noexcept {
        return "edi_n_replace_sequence" ;
    }
    void EdiNReplaceSequence::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        using keybrd::pushup ;

        VirtualCmdLine::clear() ;
        VirtualCmdLine::msgout("-- EDI REPLACE --") ;

        std::vector<unsigned char> strs{} ;
        std::vector<bool> shifts{} ;

        loop_for_keymatching([&strs, &shifts](const auto& keycodes, const bool shifted=false) {
            pushup(KEYCODE_DELETE) ;
            if(shifted) {
                pushup(KEYCODE_LSHIFT, keycodes) ;
                strs.push_back(keycodes) ;
                shifts.push_back(true) ;
            }
            else {
                pushup(keycodes) ;
                strs.push_back(keycodes) ;
                shifts.push_back(false) ;
            }
            return false ; //continue looping
        }) ;

        for(unsigned int i = 0 ; i < repeat_num - 1 ; i ++) {
            for(std::size_t stridx = 0 ; stridx < strs.size() ; stridx ++) {
                pushup(KEYCODE_DELETE) ;
                if(shifts[stridx]) pushup(KEYCODE_LSHIFT, strs[stridx]) ;
                else pushup(strs[stridx]) ;
            }
        }

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    }

    //EdiSwitchCharCase
    const std::string EdiSwitchCharCase::sname() noexcept {
        return "edi_switch_char_case" ;
    }
    void EdiSwitchCharCase::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto res = textanalyze::get_selected_text([&repeat_num] {
                for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                    keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                }
                keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
            }) ;

        for(char c : res.str) {
            if(c >= 'a' && c <= 'z') {
                keybrd::pushup(KEYCODE_LSHIFT, keycodecvt::get_keycode(c)) ;
            }
            else if(c >= 'A' && c <= 'Z') {
                constexpr char delta = 'a' - 'A' ;
                keybrd::pushup(keycodecvt::get_keycode(c + delta)) ;
            }
            else {
                auto keycode = keycodecvt::get_keycode(c) ;
                if(keycode) {
                    keybrd::pushup(keycode) ;
                    continue ;
                }
     
                keycode = keycodecvt::get_shifted_keycode(c) ;
                if(keycode) {
                    keybrd::pushup(KEYCODE_LSHIFT, keycode) ;
                    continue ;
                }
            }
        }
    }
}
