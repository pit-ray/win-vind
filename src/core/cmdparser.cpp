#include "cmdparser.hpp"

#include "errlogger.hpp"
#include "keycode.hpp"
#include "keycodedef.hpp"
#include "mode.hpp"
#include "util/container.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <algorithm>
#include <string>


namespace vind
{
    namespace core
    {
        KeySet parse_ascii_command(char onechar) {
            core::KeyCode keycode(onechar) ;
            if(keycode.empty()) {
                throw RUNTIME_EXCEPT( \
                        std::string("The character '") + \
                        onechar + "' is invalid ascii key code.") ;
            }

            //ex) A (A is divided to a and SHIFT)
            if(auto shift = core::get_shift_keycode(onechar)) {
                return KeySet{shift, keycode} ;
            }
            //ex) a
            return KeySet{keycode} ;
        }


        KeySet parse_combined_command(const std::string& inside_of_brackets) {
            KeySet keyset ;

            auto keystrset = util::split(inside_of_brackets, "-") ;
            for(auto itr = keystrset.begin() ; itr != keystrset.end() ; itr ++) {

                //If isn't begin(), regards it as ascii.
                if(itr != keystrset.begin() && itr->length() == 1) {
                    //ascii
                    for(const auto& keycode : parse_ascii_command(itr->front())) {
                        keyset.push_back(keycode) ;
                    }
                    continue ;
                }

                // regard as a specific system keycode.
                auto lowercode = util::A2a(*itr) ;

                static std::unordered_map<std::string, char> magic_ascii_{
                    {"space", ' '},
                    {"hbar",  '-'},
                    {"gt",    '>'},
                    {"lt",    '<'}
                } ;
                if(magic_ascii_.find(lowercode) != magic_ascii_.end()) {
                    for(const auto& keycode : parse_ascii_command(magic_ascii_[lowercode])) {
                        keyset.push_back(keycode) ;
                    }
                    continue ;
                }

                core::KeyCode keycode(lowercode, PREFER_SYSTEM_CODE) ;
                if(!keycode.empty()) {
                    keyset.push_back(keycode) ;
                    continue ;
                }

                if(itr == keystrset.begin()) {
                    throw RUNTIME_EXCEPT( \
                            "The beginning of a combined command of bindings " \
                            "(" + *itr + ") " \
                            "must be a system keycode (e.g. <ctrl-x>, <alt-i>).") ;
                }
                else {
                    throw RUNTIME_EXCEPT("<" + *itr + "> is invalid syntax.") ;
                }
            }

            util::remove_deplication(keyset) ;
            return keyset ;
        }


        Command parse_command(const std::string& cmdstr) {
            Command cmd ;
            for(std::size_t i = 0 ; i < cmdstr.length() ; i ++) {
                auto onechar = cmdstr[i] ;
                if(onechar != '<') {
                    cmd.push_back(parse_ascii_command(onechar)) ;
                    continue ;
                }

                auto pairpos = cmdstr.find('>', i + 1) ;
                if(pairpos == std::string::npos) {
                    throw RUNTIME_EXCEPT( \
                            cmdstr + \
                            " is bad syntax. " \
                            "It does not have a greater-than sign '>'.") ;
                }

                auto inside_cmd = cmdstr.substr(i + 1, pairpos - i - 1) ;
                cmd.push_back(parse_combined_command(inside_cmd)) ;

                i = pairpos ;
            }
            return cmd ;
        }
    }
}
