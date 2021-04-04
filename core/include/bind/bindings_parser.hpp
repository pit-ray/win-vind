#ifndef _BINDINGS_PARSER_HPP
#define _BINDINGS_PARSER_HPP

#include "bindings_matcher.hpp"
#include "mode.hpp"

namespace vind
{
    namespace bindparser
    {
        //
        // It parses a ascii, which consist of a character, as keyset class.
        // keyset_t: std::vector<unsigned char>
        //
        // Ex)
        //     'a' -> {KEYCODE_A}
        //     'A' -> {KEYCODE_SHIFT, KEYCODE_A}
        //
        const BindingsMatcher::keyset_t parse_pure_one_character_command(char onechar) ;
        //
        // It parse a combined command, which is sandwiched between '<' and '>', as keyset class.
        // keyset_t: std::vector<unsigned char>
        //
        // Note: Its argument is a inside string in brackets. If the command is "<s-a>", it is "s-a".
        //
        // Ex)
        //      "s-a"    -> {KEYCODE_SHIFT, KEYCODE_A}
        //      "ctrl-b" -> {KEYCODE_CTRL, KEYCODE_B}
        //      "x-t"    -> {KEYCODE_X, KEYCODE_T}
        //      "x-t-h"  -> {KEYCODE_X, KEYCODE_T, KEYCODE_H}
        //      "s-s"    -> {KEYCODE_SHIFT, KEYCODE_S}
        //      "c-S"    -> {KEYCODE_CTRL, KEYCODE_SHIFT, KEYCODE_S}
        //
        const BindingsMatcher::keyset_t parse_combined_command(std::string inside_of_brackets) ;

        //
        // It parses a string command as cmd_t.
        // cmd_t:: std::vector<std::vector<unsigned char>>
        //
        // Note: If it includes some mode strings,
        //       will give empty value to cmd and return value other than Mode::None.
        //
        //  Ex)
        //      abc     -> {
        //                     {KEYCODE_A},
        //                     {KEYCODE_B},
        //                     {KEYCODE_C}
        //                 }
        //
        //                 return Mode::None
        //
        //      <s-d>e  -> {
        //                     {KEYCODE_SHIFT, KEYCODE_D},
        //                     {KEYCODE_E},
        //                 }
        //
        //                 return Mode::None
        //
        //      <guin>  -> {}
        //                 return Mode::Normal
        //
        mode::Mode parse_bindings(BindingsMatcher::cmd_t& cmd, std::string cmdstr) ;

        namespace debug {
            std::string print(const BindingsMatcher::cmdlist_t& list) ;
        }
    }
}

#endif
