#include "bindings_parser.hpp"

#include "err_logger.hpp"
#include "keycode.hpp"
#include "keycode_def.hpp"
#include "mode.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <algorithm>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif


namespace vind
{
    namespace core
    {
        KeySet parse_pure_one_character_command(char onechar) {
            core::KeyCode keycode(onechar) ;
            if(keycode.empty()) {
                throw RUNTIME_EXCEPT( \
                        std::string("The character '") + \
                        onechar + "' is invalid ascii key code.") ;
            }

            if(KeyCode::is_shifted(onechar)) {  //ex) A (A is divided to a and SHIFT)
                return KeySet{KEYCODE_SHIFT, keycode} ;
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
                    for(auto& keycode : parse_pure_one_character_command(itr->front())) {
                        keyset.push_back(keycode) ;
                    }
                    continue ;
                }

                // regard as a specific system keycode.
                auto lowercode = util::A2a(*itr) ;

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
