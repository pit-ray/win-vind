#ifndef _BINDINGS_PARSER_HPP
#define _BINDINGS_PARSER_HPP

#include <string>
#include <vector>

#include "bind/base/mode.hpp"
#include "bind/bindings_def.hpp"

namespace vind
{
    namespace bindparser
    {
        //
        // It parses a ascii, which consist of a character, as keyset class.
        // KeySet: std::vector<unsigned char>
        //
        // Ex)
        //     'a' -> {KEYCODE_A}
        //     'A' -> {KEYCODE_SHIFT, KEYCODE_A}
        //
        const KeySet parse_pure_one_character_command(char onechar) ;
        //
        // It parse a combined command, which is sandwiched between '<' and '>', as keyset class.
        // KeySet: std::vector<unsigned char>
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
        const KeySet parse_combined_command(std::string inside_of_brackets) ;


        //
        // It parses a string command as Command.
        // Command:: std::vector<std::vector<unsigned char>>
        //
        //  Ex)
        //      abc     -> {
        //                     {KEYCODE_A},
        //                     {KEYCODE_B},
        //                     {KEYCODE_C}
        //                 }
        //
        //      <s-d>e  -> {
        //                     {KEYCODE_SHIFT, KEYCODE_D},
        //                     {KEYCODE_E},
        //                 }
        //
        //
        const Command parse_string_binding(std::string cmdstr) ;


        //
        // It parses a string mode with brackets.
        // Ex)
        //      <guin>  ->  Mode::Normal
        //
        // If Failed, return Mode::None.
        //
        mode::Mode parse_string_modecode(std::string modestr) ;

        namespace debug {
            std::string print(const CommandList& list) ;
        }
    }
}

#endif
