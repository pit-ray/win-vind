#include "bind/emu/edi_replace.hpp"

#include <iostream>
#include <windows.h>

#include "bind/base/char_logger.hpp"
#include "bind/base/ntype_logger.hpp"
#include "bind/base/safe_repeater.hpp"
#include "bind/emu/text_analyzer.hpp"
#include "entry.hpp"
#include "io/keybrd.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"

namespace
{
    using namespace vind ;

    inline auto is_shift(unsigned key) noexcept {
        return key == KEYCODE_SHIFT || key == KEYCODE_LSHIFT || key == KEYCODE_RSHIFT ;
    }

    template <typename FuncT>
    inline void loop_for_keymatching(FuncT&& func) {
        //reset keys downed in order to call this function.
        for(auto& key : keyabsorber::get_pressed_list()) {
            if(is_shift(key)) continue ;
            keybrd::release_keystate(key) ;
        }

        while(vind::update_background()) {
            if(keyabsorber::is_pressed(KEYCODE_ESC)) {
                return ;
            }
            auto log = keyabsorber::get_pressed_list() ;

            if(!log.is_containing(KEYCODE_SHIFT)) {
                //not shifted
                for(auto& key : log) {
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
                for(auto& key : log) {
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
    EdiNReplaceChar::EdiNReplaceChar()
    : BindedFuncCreator("edi_n_replace_char")
    {}
    void EdiNReplaceChar::sprocess(unsigned int repeat_num) {
        loop_for_keymatching([repeat_num](const auto& keycodes, bool shifted=false) {

            repeater::safe_for(repeat_num, [&keycodes, shifted] {
                keybrd::pushup(KEYCODE_DELETE) ;

                if(shifted) keybrd::pushup(KEYCODE_LSHIFT, keycodes) ;
                else keybrd::pushup(keycodes) ;
            }) ;

            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LEFT) ;
            }) ;

            return true ; //terminate looping
        }) ;
    }
    void EdiNReplaceChar::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiNReplaceChar::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiNReplaceSequence
    EdiNReplaceSequence::EdiNReplaceSequence()
    : BindedFuncCreator("edi_n_replace_sequence")
    {}
    void EdiNReplaceSequence::sprocess(unsigned int repeat_num) {
        using keybrd::pushup ;

        VirtualCmdLine::clear() ;
        VirtualCmdLine::msgout("-- EDI REPLACE --") ;

        std::vector<KeyCode> strs{} ;
        std::vector<bool> shifts{} ;

        loop_for_keymatching([&strs, &shifts](const auto& keycodes, bool shifted=false) {
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

        repeater::safe_for(repeat_num, [&strs, shifts] {
            for(std::size_t stridx = 0 ; stridx < strs.size() ; stridx ++) {
                pushup(KEYCODE_DELETE) ;
                if(shifts[stridx]) pushup(KEYCODE_LSHIFT, strs[stridx]) ;
                else pushup(strs[stridx]) ;
            }
        }) ;

        VirtualCmdLine::clear() ;
        VirtualCmdLine::refresh() ;
        VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    }
    void EdiNReplaceSequence::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiNReplaceSequence::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //EdiSwitchCharCase
    EdiSwitchCharCase::EdiSwitchCharCase()
    : BindedFuncCreator("edi_switch_char_case")
    {}
    void EdiSwitchCharCase::sprocess(unsigned int repeat_num) {
        auto res = textanalyze::get_selected_text([&repeat_num] {
                repeater::safe_for(repeat_num, [] {
                    keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_RIGHT) ;
                }) ;
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
    void EdiSwitchCharCase::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void EdiSwitchCharCase::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
