#include "bindings_parser.hpp"

#include <algorithm>
#include <sstream>
#include <string>

#include "err_logger.hpp"

#include "keycodecvt.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include "mode.hpp"

namespace vind
{
    namespace core
    {
        //
        // It parses a ascii, which consist of a character, as keyset class.
        // KeySet: std::vector<KeyCode>
        //
        // Ex)
        //     'a' -> {KEYCODE_A}
        //     'A' -> {KEYCODE_SHIFT, KEYCODE_A}
        //
        KeySet parse_pure_one_character_command(char onechar) {
            //ascii
            if(auto keycode = get_keycode(onechar)) { //ex) a
                return KeySet{keycode} ;
            }

            //shifted ascii
            if(auto keycode = get_shifted_keycode(onechar)) { //ex) A (A is divided to a and SHIFT)
                return KeySet{KEYCODE_SHIFT, keycode} ;
            }
            throw RUNTIME_EXCEPT(std::string("The character '") + onechar + "' is invalid ascii key code.") ;
        }

        //
        // It parse a combined command, which is sandwiched between '<' and '>', as keyset class.
        // KeySet: std::vector<KeyCode>
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
        KeySet parse_combined_command(const std::string& inside_of_brackets) {
            KeySet keyset ;

            auto keystrset = util::split(inside_of_brackets, "-") ;
            for(auto code = keystrset.begin() ; code != keystrset.end() ; code ++) {

                //If isn't begin(), regards it as ascii.
                if(code != keystrset.begin() && code->length() == 1) {
                    //ascii
                    for(auto& keycode : parse_pure_one_character_command(code->front())) {
                        keyset.push_back(keycode) ;
                    }
                    continue ;
                }

                // regard as a specific system keycode.
                auto lowercode = util::A2a(*code) ;

                if(auto keycode = get_keycode_of_magic(lowercode)) {
                    keyset.push_back(keycode) ;
                    continue ;
                }

                if(auto keycode = get_sys_keycode(lowercode)) {
                    keyset.push_back(keycode) ;
                    continue ;
                }

                if(code == keystrset.begin()) {
                    PRINT_ERROR("The beginning of a combined command of bindings (" +
                            *code  + ") must be a system keycode (e.g. <ctrl-x>, <alt-i>).") ;
                }
                else {
                    PRINT_ERROR("<" + *code + "> is invalid syntax.") ;
                }
            }

            util::remove_deplication(keyset) ;
            std::sort(keyset.begin(), keyset.end()) ;
            return keyset ; //RVO
        }

        Command parse_string_binding(const std::string& cmdstr) {
            Command cmd ;
            for(std::size_t i = 0 ; i < cmdstr.length() ; i ++) {
                auto onechar = cmdstr[i] ;
                if(onechar != '<') {
                    cmd.push_back(parse_pure_one_character_command(onechar)) ;
                    continue ;
                }

                auto pairpos = cmdstr.find('>', i + 1) ;
                if(pairpos == std::string::npos) {
                    throw RUNTIME_EXCEPT(cmdstr + " is bad syntax. It does not have a greater-than sign '>'.") ;
                }

                auto inside_cmd = cmdstr.substr(i + 1, pairpos - i - 1) ;
                cmd.push_back(parse_combined_command(inside_cmd)) ;

                i = pairpos ;
            }
            return cmd ;
        }

        Mode parse_string_modecode(
                const std::string& modestr,
                std::string root) {
            if(modestr.front() == '<' && modestr.back() == '>') {
                auto inside = util::A2a(modestr.substr(1, modestr.size() - 2)) ;
                auto prefix = inside.substr(0, inside.find(root)) ;
                return parse_mode_prefix(prefix) ;
            }
            return Mode::UNDEFINED ;
        }

#ifdef DEBUG
        std::string print(const Command& cmd) {
            std::stringstream ss ;
            for(const auto& keyset : cmd) {
                if(keyset.size() == 1) {
                    auto name = get_keycode_name(keyset.front()) ;
                    if(name.size() == 1) {
                        ss << name ;
                    }
                    else {
                        ss << "<" << name << ">" ;
                    }
                }
                else {
                    ss << "<" ;
                    for(auto keyitr = keyset.cbegin() ; keyitr != keyset.cend() ; keyitr ++) {
                        if(keyset.size() > 1 && keyitr != keyset.cbegin()) {
                            ss << "-" ;
                        }
                        ss << get_keycode_name(*keyitr) ;
                    }
                    if(keyset.size() > 1) {
                        ss << ">" ;
                    }
                }
            }
            return ss.str() ;
        }
        std::string print(const CommandList& list) {
            std::stringstream ss ;

            ss << "[" ;
            for(auto cmditr = list.cbegin() ; cmditr != list.cend() ; cmditr ++) {
                if(list.size() > 1 && cmditr != list.cbegin()) {
                    ss << ", " ;
                }

                ss << print(*cmditr) ;
            }
            ss << "]" ;

            return ss.str() ;
        }
#endif
    }
}
