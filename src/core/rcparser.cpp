#include "rcparser.hpp"

#include "mode.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/type_traits.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>


namespace vind
{
    namespace core {
        void remove_dbquote_comment(std::string& line) {
            // If a line begins with a double quote, it are considered as a blank line.
            // 
            // For example,
            //  " foo "bar"
            // { "foo "bar"} is comment.
            auto start_pos = line.find_first_not_of(" ") ;
            if(start_pos == std::string::npos || line[start_pos] == '\"') {
                line.clear() ;
                return ;
            }

            // If there are double quotes in the middle of a line,
            // the last double quote without a pair is considered a comment.
            //
            // For example,
            // foo "bar" " hoge
            // {foo "bar" } is valid, {" hoge} is comment.
            //
            while(true) {
                auto left_dquote = line.find_first_of("\"", start_pos) ;
                if(left_dquote == std::string::npos) {
                    break ;
                }
                auto right_dquote = line.find_first_of("\"", left_dquote + 1) ;
                if(right_dquote == std::string::npos) {
                    // remove comment
                    line.erase(left_dquote) ;
                    break ;
                }
                else {
                    start_pos = right_dquote + 1 ;
                }
            }
        }

        std::pair<std::string, std::string> divide_cmd_and_args(
                const std::string& line) {
            if(line.empty()) {
                return std::make_pair("", "") ;
            }

            auto cmd_first = line.find_first_not_of(" ") ;
            if(cmd_first == std::string::npos) {
                return std::make_pair("", "") ;
            }
            auto cmd_last = line.find_first_of(" ", cmd_first + 1) - 1 ;
            if(cmd_last == std::string::npos) {
                return std::make_pair(line.substr(cmd_first), "") ;
            }
            auto cmd = line.substr(cmd_first, cmd_last - cmd_first + 1) ;

            auto args_first = line.find_first_not_of(" ", cmd_last + 1) ;
            if(args_first == std::string::npos) {
                return std::make_pair(cmd, "") ;
            }
            auto args_last = line.find_last_not_of(" ") ;

            auto args = line.substr(args_first, args_last - args_first + 1) ;
            return std::make_pair(cmd, args) ;
        }

        std::string extract_single_arg(const std::string& args) {
            if(args.empty()) {
                return "" ;
            }

            auto first = args.find_first_not_of(" ") ;
            if(first == std::string::npos) {
                return "" ;
            }
            auto last = args.find_last_not_of(" ") ;
            auto arg = args.substr(first, last - first + 1) ;
            if(arg.find(" ") != std::string::npos) {
                throw RUNTIME_EXCEPT("It was parsed as a single argument, but it contains multiple arguments.") ;
            }
            return arg ;
        }

        std::pair<std::string, std::string> extract_double_args(
                const std::string& args) {
            if(args.empty()) {
                return std::make_pair("", "") ;
            }

            auto arg1_first = args.find_first_not_of(" ") ;
            if(arg1_first == std::string::npos) {
                return std::make_pair("", "") ;
            }

            auto delimiter = args.find_first_of(" ", arg1_first + 1) ;
            if(delimiter == std::string::npos) {
                return std::make_pair(args.substr(arg1_first), "") ;
            }
            auto arg1 = args.substr(arg1_first, delimiter - arg1_first) ;

            auto arg2_first = args.find_first_not_of(" ", delimiter + 1) ;
            if(arg2_first == std::string::npos) {
                return std::make_pair(arg1, "") ;
            }
            auto arg2_last = args.find_last_not_of(" ") ;
            auto arg2 = args.substr(arg2_first, arg2_last - arg2_first + 1) ;

            return std::make_pair(arg1, arg2) ;
        }

        std::pair<std::string, std::string> divide_key_and_value(
                const std::string& args,
                std::string delimiter) {
            if(args.empty()) {
                return std::make_pair("", "") ;
            }
            if(delimiter.empty()) {
                return std::make_pair(args, "") ;
            }

            auto eq_pos = args.find_first_of(delimiter) ;
            if(eq_pos == std::string::npos) {
                return std::make_pair(args, "") ;
            }
            auto key = extract_single_arg(args.substr(0, eq_pos)) ;

            auto val_area = args.substr(eq_pos + 1) ;
            auto val_first = val_area.find_first_not_of(" ") ;
            if(val_first == std::string::npos) {
                return std::make_pair(key, "") ;
            }
            auto val_last = val_area.find_last_not_of(" ") ;

            auto val = val_area.substr(val_first, val_last - val_first + 1) ;
            return std::make_pair(key, val) ;
        }

        std::pair<std::string, std::string> divide_prefix_and_cmd(
                const std::string& str,
                const std::string& split_keyword) {

            auto pos = str.find_last_of(split_keyword) ;
            if(pos == std::string::npos) {
                return std::make_pair("", str) ;
            }
            return std::make_pair(
                    str.substr(0, pos),
                    str.substr(pos)) ;
        }


        RunCommandsIndex parse_run_command(const std::string& strcmd) {
            static auto parser = [] {
                std::unordered_map<std::string, RunCommandsIndex> tmp {
                    {"se",          RunCommandsIndex::SET},
                    {"set",         RunCommandsIndex::SET},

                    {"command",     RunCommandsIndex::COMMAND},
                    {"comman",      RunCommandsIndex::COMMAND},
                    {"comma",       RunCommandsIndex::COMMAND},
                    {"comm",        RunCommandsIndex::COMMAND},
                    {"com",         RunCommandsIndex::COMMAND},

                    {"delcommand",  RunCommandsIndex::DELCOMMAND},
                    {"delcomman",   RunCommandsIndex::DELCOMMAND},
                    {"delcomma",    RunCommandsIndex::DELCOMMAND},
                    {"delcomm",     RunCommandsIndex::DELCOMMAND},
                    {"delcom",      RunCommandsIndex::DELCOMMAND},
                    {"delco",       RunCommandsIndex::DELCOMMAND},
                    {"delc",        RunCommandsIndex::DELCOMMAND},


                    {"comclear",    RunCommandsIndex::COMCLEAR},
                    {"comclea",     RunCommandsIndex::COMCLEAR},
                    {"comcle",      RunCommandsIndex::COMCLEAR},
                    {"comcl",       RunCommandsIndex::COMCLEAR},
                    {"comc",        RunCommandsIndex::COMCLEAR},

                    {"source",      RunCommandsIndex::SOURCE},
                    {"sourc",       RunCommandsIndex::SOURCE},
                    {"sour",        RunCommandsIndex::SOURCE},
                    {"sou",         RunCommandsIndex::SOURCE},
                    {"so",          RunCommandsIndex::SOURCE},
                } ;

                for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                    auto mode_prefix = mode_to_prefix(i) ;

                    // map
                    auto map_rc_index = util::enum_or(i, RunCommandsIndex::MASK_MAP) ;
                    tmp[mode_prefix + "map"] = map_rc_index ;
                    tmp[mode_prefix + "ma"]  = map_rc_index ;
                    tmp[mode_prefix + "m"]   = map_rc_index ;

                    // noremap
                    auto noremap_rc_index = util::enum_or(i, RunCommandsIndex::MASK_NOREMAP) ;
                    tmp[mode_prefix + "noremap"] = noremap_rc_index ;
                    tmp[mode_prefix + "norema"]  = noremap_rc_index ;
                    tmp[mode_prefix + "norem"]   = noremap_rc_index ;
                    tmp[mode_prefix + "nore"]    = noremap_rc_index ;
                    tmp[mode_prefix + "nor"]     = noremap_rc_index ;
                    tmp[mode_prefix + "no"]      = noremap_rc_index ;
                    if(mode_prefix.size() > 1) {
                        tmp[mode_prefix + "n"]   = noremap_rc_index ;
                    }

                    // unmap
                    auto unmap_rc_index = util::enum_or(i, RunCommandsIndex::MASK_UNMAP) ;
                    tmp[mode_prefix + "unmap"] = unmap_rc_index ;
                    tmp[mode_prefix + "unma"]  = unmap_rc_index ;
                    tmp[mode_prefix + "unm"]   = unmap_rc_index ;
                    tmp[mode_prefix + "un"]    = unmap_rc_index ;
                    if(mode_prefix.size() > 1) {
                        tmp[mode_prefix + "u"] = unmap_rc_index ;
                    }

                    // mapclear
                    auto mapclear_rc_index = util::enum_or(i, RunCommandsIndex::MASK_MAPCLEAR) ;
                    tmp[mode_prefix + "mapclear"] = mapclear_rc_index ;
                    tmp[mode_prefix + "mapclea"]  = mapclear_rc_index ;
                    tmp[mode_prefix + "mapcle"]   = mapclear_rc_index ;
                    tmp[mode_prefix + "mapcl"]    = mapclear_rc_index ;
                    tmp[mode_prefix + "mapc"]     = mapclear_rc_index ;
                }
                return tmp ;
            }() ;

            try {
                return parser.at(strcmd) ;
            }
            catch(const std::out_of_range&) {
                return RunCommandsIndex::UNDEFINED ;
            }
        }
    }
}
