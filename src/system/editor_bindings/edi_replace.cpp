#include "edi_replace.hpp"

#include <windows.h>
#include <iostream>

#include "keybrd_eventer.hpp"
#include "key_absorber.hpp"
#include "vkc_converter.hpp"
#include "virtual_cmd_line.hpp"
#include "system.hpp"

namespace EREPUtility {

    inline static auto is_shift(const unsigned key) noexcept {
        return key == VKC_SHIFT || key == VKC_LSHIFT || key == VKC_RSHIFT ;
    }

    template <typename FuncT>
    inline static bool _is_loop_for_input(FuncT&& func) {
        //reset keys downed in order to call this function.
        for(const auto& key : KeyAbsorber::get_pressed_list()) {
            if(is_shift(key)) continue ;
            if(!KeybrdEventer::release_keystate(key)) {
                return false ;
            }
        }

        MSG msg ;
        while(System::update_options()) {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg) ;
                DispatchMessage(&msg) ;
            }
            if(KeyAbsorber::is_pressed(VKC_ESC)) {
                return true ;
            }
            const auto log = KeyAbsorber::get_pressed_list() ;

            if(!log.is_containing(VKC_SHIFT)) {
                //not shifted
                for(const auto& key : log) {
                    //For example, if replace by 'i' and 'i' key is downed,
                    //immediately will call "insert-mode", so release 'i'.
                    if(!KeybrdEventer::release_keystate(key)) {
                        return false ;
                    }
                    if(!VKCConverter::get_ascii(key)) {
                        continue ;
                    }
                    if(func(key)) {
                        return true ;
                    }
                }
            }
            else {
                //shifted
                for(const auto& key : log) {
                    if(is_shift(key)) continue ;
                    if(!KeybrdEventer::release_keystate(key)) {
                        return false ;
                    }
                    if(!VKCConverter::get_shifted_ascii(key)) {
                        continue ;
                    }
                    if(func(key, true)) {
                        return true ;
                    }
                }
            }
            Sleep(10) ;
        }

        return true ;
    }
}


//EdiNRplaceChar
const std::string EdiNReplaceChar::sname() noexcept
{
    return "edi_n_replace_char" ;
}
bool EdiNReplaceChar::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    return EREPUtility::_is_loop_for_input([](const auto& vkcs, const bool shifted=false) {
        if(!KeybrdEventer::pushup(VKC_DELETE)) {
            return false ;
        }

        if(shifted) {
            if(!KeybrdEventer::pushup(VKC_LSHIFT, vkcs)) {
                return false ;
            }
        }
        else {
            if(!KeybrdEventer::pushup(vkcs)) {
                return false ;
            }
        }
        if(!KeybrdEventer::pushup(VKC_LEFT)) {
            return false ;
        }
        return true ; //terminate looping
    }) ;
}


//EdiNReplaceSequence
const std::string EdiNReplaceSequence::sname() noexcept
{
    return "edi_n_replace_sequence" ;
}
bool EdiNReplaceSequence::sprocess(const bool first_call)
{
    if(!first_call) {
        return true ;
    }

    VirtualCmdLine::clear() ;
    VirtualCmdLine::msgout("-- EDI REPLACE --") ;
    if(!EREPUtility::_is_loop_for_input([](const auto& vkcs, const bool shifted=false) {
        if(!KeybrdEventer::pushup(VKC_DELETE)) {
            return false ;
        }

        if(shifted) {
            if(!KeybrdEventer::pushup(VKC_LSHIFT, vkcs)) {
                return false ;
            }
        }
        else {
            if(!KeybrdEventer::pushup(vkcs)) {
                return false ;
            }
        }
        return false ; //continue looping

    })) {
        return false ;
    }

    VirtualCmdLine::clear() ;
    VirtualCmdLine::refresh() ;
    VirtualCmdLine::msgout("-- EDI NORMAL --") ;
    return true ;
}