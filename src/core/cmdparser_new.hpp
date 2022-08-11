#ifndef _CMDPARSER_HPP
#define _CMDPARSER_HPP

#include "cmdunit.hpp"


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
        CmdUnitSet parse_ascii_command(char onechar) ;


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
        CmdUnitSet parse_combined_command(const std::string& inside_of_brackets) ;


        /**
         * It parses a command to a vector of the command unit.
         * Each element of the returned vector is based on polymorphism,
         * so the original class may be InternalCmdUnit, ExternalCmdUnit,
         * or FunctionalCmdUnit.
         *
         * e.g.
         *   a<c-j>            ->    ICU('a'), ICU('ctrl', 'j')
         *   b{jj}             ->    ICU('b'), ECU('j'), ECU('j')
         *  <easy_click_left>  -> FCU('easy_click_left')
         *   {{}}              ->    ECU('{'), ICU('}')
         *
         *   The {} denotes the external scope cmd unit and it matches with the first appeared bracket.
         *   For example, {{}} has the following correspondence of brackets.
         *   {  {   }   }
         *   |      |
         *   +______+
         *   external scope correspondence
         */
        std::vector<CmdUnit::SPtr> parse_command(const std::string& cmdstr) ;
    }
}

#endif
