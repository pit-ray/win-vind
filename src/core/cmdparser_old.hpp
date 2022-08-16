#ifndef _BINDINGS_PARSER_HPP
#define _BINDINGS_PARSER_HPP

#include <string>
#include <vector>

#include "cmdunit.hpp"
#include "keycode.hpp"
#include "mode.hpp"

namespace vind
{
    namespace core
    {
        /**
         * NOTE: It parses a ascii, which consist of a character,
         *       as keyset class.
         * Ex)
         *      'a' -> {KEYCODE_A}
         *      'A' -> {KEYCODE_SHIFT, KEYCODE_A}
         *
         * @return (KeySet(std::vector<KeyCode>))
         */
        KeySet parse_ascii_command_old(char onechar) ;

        /**
         * NOTE: It parse a combined command, which is sandwiched 
         *       between '<' and '>', as keyset class. Its argument
         *       is a inside string in brackets. If the command
         *       is "<s-a>", it is "s-a".
         * Ex)
         *      "s-a"    -> {KEYCODE_SHIFT, KEYCODE_A}
         *      "ctrl-b" -> {KEYCODE_CTRL, KEYCODE_B}
         *      "x-t"    -> {KEYCODE_X, KEYCODE_T}
         *      "x-t-h"  -> {KEYCODE_X, KEYCODE_T, KEYCODE_H}
         *      "s-s"    -> {KEYCODE_SHIFT, KEYCODE_S}
         *      "c-S"    -> {KEYCODE_CTRL, KEYCODE_SHIFT, KEYCODE_S}
         *
         * @return (KeySet(std::vector<KeyCode>))
         */
        KeySet parse_combined_command_old(const std::string& inside_of_brackets) ;


        /**
         * NOTE: It parses a string command as Command.
         *  Ex)
         *      abc     -> {
         *                     {KEYCODE_A},
         *                     {KEYCODE_B},
         *                     {KEYCODE_C}
         *                 }
         *
         *      <s-d>e  -> {
         *                     {KEYCODE_SHIFT, KEYCODE_D},
         *                     {KEYCODE_E},
         *                 }
         *
         * @return (Command(std::vector<std::vector<KeyCode>>))
         *
         */
        Command parse_command_old(const std::string& cmdstr) ;
    }
}

#endif
