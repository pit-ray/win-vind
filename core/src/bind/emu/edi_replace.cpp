#include "edi_replace.hpp"

#include <windows.h>
#include <iostream>

#include "io/keybrd_eventer.hpp"
#include "key/key_absorber.hpp"
#include "key/virtual_key_fwd.hpp"
#include "key/vkc_converter.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/text_analyzer.hpp"
#include "utility.hpp"
#include "win_vind.hpp"

namespace
{
    using namespace vind ;

    inline auto is_shift(const unsigned key) noexcept {
        return key == VKC_SHIFT || key == VKC_LSHIFT || key == VKC_RSHIFT ;
    }

    template <typename FuncT>
    inline void loop_for_keymatching(FuncT&& func) {
        //reset keys downed in order to call this function.
        for(const auto& key : KeyAbsorber::get_pressed_list()) {
            if(is_shift(key)) continue ;
            KeybrdEventer::release_keystate(key) ;
        }

        while(vind::update_background()) {
            if(KeyAbsorber::is_pressed(VKC_ESC)) {
                return ;
            }
            const auto log = KeyAbsorber::get_pressed_list() ;

            if(!log.is_containing(VKC_SHIFT)) {
                //not shifted
                for(const auto& key : log) {
                    //For example, if replace by 'i' and 'i' key is downed,
                    //immediately will call "insert-mode", so release 'i'.
                    KeybrdEventer::release_keystate(key) ;

                    if(!VKCConverter::get_ascii(key)) {
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
                    KeybrdEventer::release_keystate(key) ;
                    if(!VKCConverter::get_shifted_ascii(key)) {
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
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        loop_for_keymatching([repeat_num](const auto& vkcs, const bool shifted=false) {

            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                KeybrdEventer::pushup(VKC_DELETE) ;

                if(shifted) KeybrdEventer::pushup(VKC_LSHIFT, vkcs) ;
                else KeybrdEventer::pushup(vkcs) ;
            }

            for(unsigned int i = 0 ; i < repeat_num ; i ++)
                KeybrdEventer::pushup(VKC_LEFT) ;

            return true ; //terminate looping
        }) ;
    }


    //EdiNReplaceSequence
    const std::string EdiNReplaceSequence::sname() noexcept {
        return "edi_n_replace_sequence" ;
    }
    void EdiNReplaceSequence::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        using KeybrdEventer::pushup ;

        VirtualCmdLine::clear() ;
        VirtualCmdLine::msgout("-- EDI REPLACE --") ;

        std::vector<unsigned char> strs{} ;
        std::vector<bool> shifts{} ;

        loop_for_keymatching([&strs, &shifts](const auto& vkcs, const bool shifted=false) {
            pushup(VKC_DELETE) ;
            if(shifted) {
                pushup(VKC_LSHIFT, vkcs) ;
                strs.push_back(vkcs) ;
                shifts.push_back(true) ;
            }
            else {
                pushup(vkcs) ;
                strs.push_back(vkcs) ;
                shifts.push_back(false) ;
            }
            return false ; //continue looping
        }) ;

        for(unsigned int i = 0 ; i < repeat_num - 1 ; i ++) {
            for(std::size_t stridx = 0 ; stridx < strs.size() ; stridx ++) {
                pushup(VKC_DELETE) ;
                if(shifts[stridx]) pushup(VKC_LSHIFT, strs[stridx]) ;
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
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto res = TextAnalyzer::get_selected_text([&repeat_num] {
                for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                    KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
                }
                KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
            }) ;

        for(char c : res.str) {
            if(c >= 'a' && c <= 'z') {
                KeybrdEventer::pushup(VKC_LSHIFT, VKCConverter::get_vkc(c)) ;
            }
            else if(c >= 'A' && c <= 'Z') {
                static constexpr char delta = 'a' - 'A' ;
                KeybrdEventer::pushup(VKCConverter::get_vkc(c + delta)) ;
            }
            else {
                auto vkc = VKCConverter::get_vkc(c) ;
                if(vkc) {
                    KeybrdEventer::pushup(vkc) ;
                    continue ;
                }
     
                vkc = VKCConverter::get_shifted_vkc(c) ;
                if(vkc) {
                    KeybrdEventer::pushup(VKC_LSHIFT, vkc) ;
                    continue ;
                }
            }
        }
    }
}
