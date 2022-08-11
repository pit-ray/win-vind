#include "cmdparser_new.hpp"

#include "bind/bindinglist.hpp"
#include "cmdunit.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <sstream>
#include <unordered_map>
#include <vector>


namespace vind
{
    namespace core
    {
        CmdUnitSet parse_ascii_command(char onechar) {
            core::KeyCode keycode(onechar) ;
            if(keycode.empty()) {
                std::stringstream ss ;
                ss << "The character '" ;
                ss << onechar ;
                ss << "' is invalid ascii key code." ;
                throw RUNTIME_EXCEPT(ss.str()) ;
            }

            // The upper-case character is divided to lower-case character and SHIFT.
            if(auto shift = core::get_shift_keycode(onechar)) {
                return CmdUnitSet{shift, keycode} ;
            }

            // Only lower-case character.
            return CmdUnitSet{keycode} ;
        }

        CmdUnitSet parse_combined_command(const std::string& inside_of_brackets) {
            CmdUnitSet keyset ;

            auto keystrset = util::split(inside_of_brackets, "-") ;
            for(auto itr = keystrset.begin() ; itr != keystrset.end() ; itr ++) {

                //If isn't begin(), regards it as ascii.
                if(itr != keystrset.begin() && itr->length() == 1) {
                    //ascii
                    for(const auto& keycode : parse_ascii_command(itr->front())) {
                        keyset.insert(keycode) ;
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
                        keyset.insert(keycode) ;
                    }
                    continue ;
                }

                core::KeyCode keycode(lowercode, PREFER_SYSTEM_CODE) ;
                if(!keycode.empty()) {
                    keyset.insert(keycode) ;
                    continue ;
                }

                std::stringstream ss ;
                if(itr == keystrset.begin()) {
                    ss << "The beginning of a combined command of bindings " ;
                    ss << "(" << *itr << ") " ;
                    ss << "must be a system keycode (e.g. <ctrl-x>, <alt-i>)." ;
                }
                else {
                    ss << "<" << *itr << "> is invalid syntax." ;
                }
                throw RUNTIME_EXCEPT(ss.str()) ;
            }

            return keyset ;
        }

        std::vector<CmdUnit::SPtr> parse_command(const std::string& strcmd) {
            std::vector<CmdUnit::SPtr> parsed ;

            int start = 0 ;
            while(start < strcmd.length()) {
                int next = strcmd.length() ;

                // scope_length denotes CmdUnit scope range, internal or external.
                // e.g.
                //   ab{jj}j
                //   ab are the internal scope and scope_length is 2.
                //   jj are the external scope and scope_length is 2.
                //   j are the internal scope and scope_length is 1.
                auto scope_length = strcmd.length() - start ;

                bool for_external = false ;
                if(strcmd[start] == '{') {
                    // When the 'start' is pointing to the beginning of the external scope.
                    // e.g.
                    //   ab{jj}j
                    //     ^
                    //   start
                    auto scope_end = strcmd.find_first_of('}', start + 1) ;
                    if(scope_end != std::string::npos) {
                        start += 1 ;
                        scope_length = scope_end - start ;
                        next = scope_end + 1 ;
                        for_external = true ;
                    }
                }
                else {
                    // If the command has the external scope and the 'start'
                    // is pointing to a position before the external scope.
                    // e.g.
                    //   ab{jj}j
                    //   ^
                    // start
                    auto scope_start = strcmd.find_first_of('{', start + 1) ;
                    if(scope_start != std::string::npos) {
                        auto scope_end = strcmd.find_first_of('}', scope_start + 1) ;
                        if(scope_end != std::string::npos) {
                            scope_length = scope_start - start ;
                            next = scope_start ;
                        }
                    }
                }

                for(std::size_t shift = 0 ; shift < scope_length ; shift ++) {
                    auto i = start + shift ;
                    auto c = strcmd[i] ;
                    if(c != '<') {
                        auto unitset = parse_ascii_command(c) ;
                        if(for_external) {
                            parsed.push_back(
                                std::make_shared<ExternalCmdUnit>(std::move(unitset))) ;
                        }
                        else {
                            parsed.push_back(
                                std::make_shared<InternalCmdUnit>(std::move(unitset))) ;
                        }
                        continue ;
                    }

                    auto pairpos = strcmd.find('>', i + 1) ;
                    if(pairpos == std::string::npos) {
                        std::stringstream ss ;
                        ss << strcmd << " is bad syntax. " ;
                        ss << "It does not have a greater-than sign '>'." ;
                        throw RUNTIME_EXCEPT(ss.str()) ;
                    }

                    auto inside_cmd = strcmd.substr(i + 1, pairpos - i - 1) ;
                    if(auto func = bind::ref_global_func_byname(inside_cmd)) {
                        parsed.push_back(
                            std::make_shared<FunctionalCmdUnit>(std::move(func))) ;
                    }
                    else {
                        auto unitset = parse_combined_command(inside_cmd) ;
                        if(for_external) {
                            parsed.push_back(
                                std::make_shared<ExternalCmdUnit>(std::move(unitset))) ;
                        }
                        else {
                            parsed.push_back(
                                std::make_shared<InternalCmdUnit>(std::move(unitset))) ;
                        }
                    }

                    // Forward the i indicator to the position after <foobar>.
                    // e.g.
                    //   a{<b>c}d
                    //        ^
                    //     start + shift
                    shift += pairpos - i ;
                }

                start = next ;
            }
            return parsed ;
        }
    }
}
