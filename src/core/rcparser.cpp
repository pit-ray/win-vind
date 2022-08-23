#include "rcparser.hpp"

#include "mode.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

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

                    {"map",         RunCommandsIndex::MAP},
                    {"ma",          RunCommandsIndex::MAP},
                    {"m",           RunCommandsIndex::MAP},

                    {"gmap",        RunCommandsIndex::MAP_GUI},
                    {"gma",         RunCommandsIndex::MAP_GUI},
                    {"gm",          RunCommandsIndex::MAP_GUI},

                    {"emap",        RunCommandsIndex::MAP_EDI},
                    {"ema",         RunCommandsIndex::MAP_EDI},
                    {"em",          RunCommandsIndex::MAP_EDI},

                    {"nmap",        RunCommandsIndex::MAP_N},
                    {"nma",         RunCommandsIndex::MAP_N},
                    {"nm",          RunCommandsIndex::MAP_N},

                    {"vmap",        RunCommandsIndex::MAP_V},
                    {"vma",         RunCommandsIndex::MAP_V},
                    {"vm",          RunCommandsIndex::MAP_V},

                    {"noremap",     RunCommandsIndex::NOREMAP},
                    {"norema",      RunCommandsIndex::NOREMAP},
                    {"norem",       RunCommandsIndex::NOREMAP},
                    {"nore",        RunCommandsIndex::NOREMAP},
                    {"nor",         RunCommandsIndex::NOREMAP},
                    {"no",          RunCommandsIndex::NOREMAP},

                    {"gnoremap",    RunCommandsIndex::NOREMAP_GUI},
                    {"gnorema",     RunCommandsIndex::NOREMAP_GUI},
                    {"gnorem",      RunCommandsIndex::NOREMAP_GUI},
                    {"gnore",       RunCommandsIndex::NOREMAP_GUI},
                    {"gnor",        RunCommandsIndex::NOREMAP_GUI},
                    {"gno",         RunCommandsIndex::NOREMAP_GUI},

                    {"enoremap",    RunCommandsIndex::NOREMAP_EDI},
                    {"enorema",     RunCommandsIndex::NOREMAP_EDI},
                    {"enorem",      RunCommandsIndex::NOREMAP_EDI},
                    {"enore",       RunCommandsIndex::NOREMAP_EDI},
                    {"enor",        RunCommandsIndex::NOREMAP_EDI},
                    {"eno",         RunCommandsIndex::NOREMAP_EDI},

                    {"nnoremap",    RunCommandsIndex::NOREMAP_N},
                    {"nnorema",     RunCommandsIndex::NOREMAP_N},
                    {"nnorem",      RunCommandsIndex::NOREMAP_N},
                    {"nnore",       RunCommandsIndex::NOREMAP_N},
                    {"nnor",        RunCommandsIndex::NOREMAP_N},
                    {"nno",         RunCommandsIndex::NOREMAP_N},

                    {"vnoremap",    RunCommandsIndex::NOREMAP_V},
                    {"vnorema",     RunCommandsIndex::NOREMAP_V},
                    {"vnorem",      RunCommandsIndex::NOREMAP_V},
                    {"vnore",       RunCommandsIndex::NOREMAP_V},
                    {"vnor",        RunCommandsIndex::NOREMAP_V},
                    {"vno",         RunCommandsIndex::NOREMAP_V},

                    {"unmap",       RunCommandsIndex::UNMAP},
                    {"unma",        RunCommandsIndex::UNMAP},
                    {"unm",         RunCommandsIndex::UNMAP},
                    {"un",          RunCommandsIndex::UNMAP},

                    {"gunmap",      RunCommandsIndex::UNMAP_GUI},
                    {"gunma",       RunCommandsIndex::UNMAP_GUI},
                    {"gunm",        RunCommandsIndex::UNMAP_GUI},
                    {"gun",         RunCommandsIndex::UNMAP_GUI},

                    {"eunmap",      RunCommandsIndex::UNMAP_EDI},
                    {"eunma",       RunCommandsIndex::UNMAP_EDI},
                    {"eunm",        RunCommandsIndex::UNMAP_EDI},
                    {"eun",         RunCommandsIndex::UNMAP_EDI},

                    {"nunmap",      RunCommandsIndex::UNMAP_N},
                    {"nunma",       RunCommandsIndex::UNMAP_N},
                    {"nunm",        RunCommandsIndex::UNMAP_N},
                    {"nun",         RunCommandsIndex::UNMAP_N},

                    {"vunmap",      RunCommandsIndex::UNMAP_V},
                    {"vunma",       RunCommandsIndex::UNMAP_V},
                    {"vunm",        RunCommandsIndex::UNMAP_V},
                    {"vun",         RunCommandsIndex::UNMAP_V},

                    {"mapclear",    RunCommandsIndex::MAPCLEAR},
                    {"mapclea",     RunCommandsIndex::MAPCLEAR},
                    {"mapcle",      RunCommandsIndex::MAPCLEAR},
                    {"mapcl",       RunCommandsIndex::MAPCLEAR},
                    {"mapc",        RunCommandsIndex::MAPCLEAR},

                    {"gmapclear",   RunCommandsIndex::MAPCLEAR_GUI},
                    {"gmapclea",    RunCommandsIndex::MAPCLEAR_GUI},
                    {"gmapcle",     RunCommandsIndex::MAPCLEAR_GUI},
                    {"gmapcl",      RunCommandsIndex::MAPCLEAR_GUI},
                    {"gmapc",       RunCommandsIndex::MAPCLEAR_GUI},

                    {"emapclear",   RunCommandsIndex::MAPCLEAR_EDI},
                    {"emapclea",    RunCommandsIndex::MAPCLEAR_EDI},
                    {"emapcle",     RunCommandsIndex::MAPCLEAR_EDI},
                    {"emapcl",      RunCommandsIndex::MAPCLEAR_EDI},
                    {"emapc",       RunCommandsIndex::MAPCLEAR_EDI},

                    {"nmapclear",   RunCommandsIndex::MAPCLEAR_N},
                    {"nmapclea",    RunCommandsIndex::MAPCLEAR_N},
                    {"nmapcle",     RunCommandsIndex::MAPCLEAR_N},
                    {"nmapcl",      RunCommandsIndex::MAPCLEAR_N},
                    {"nmapc",       RunCommandsIndex::MAPCLEAR_N},

                    {"vmapclear",   RunCommandsIndex::MAPCLEAR_V},
                    {"vmapclea",    RunCommandsIndex::MAPCLEAR_V},
                    {"vmapcle",     RunCommandsIndex::MAPCLEAR_V},
                    {"vmapcl",      RunCommandsIndex::MAPCLEAR_V},
                    {"vmapc",       RunCommandsIndex::MAPCLEAR_V},
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
