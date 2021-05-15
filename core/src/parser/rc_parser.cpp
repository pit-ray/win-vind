#include "parser/rc_parser.hpp"

#include "mode.hpp"
#include "util/def.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace vind
{
    namespace rcparser {
        void remove_dbquote_comment(std::string& line) {
            std::size_t start_pos = 0 ;
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

            auto arg2_first = args.find_first_not_of(" ", delimiter + 1) ;
            if(arg2_first == std::string::npos) {
                return std::make_pair(args.substr(arg1_first, delimiter - arg1_first), "") ;
            }
            auto arg2_last = args.find_last_not_of(" ") ;
            auto arg2 = args.substr(arg2_first, arg2_last - arg2_first + 1) ;
            if(arg2.find(" ") != std::string::npos) {
                throw RUNTIME_EXCEPT("It was parsed as double arguments, but it contains bigger than two.") ;
            }

            auto arg1 = args.substr(arg1_first, delimiter - arg1_first) ;
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

            auto val_area = args.substr(eq_pos + 1) ;
            auto val_first = val_area.find_first_not_of(" ") ;
            if(val_first == std::string::npos) {
                throw RUNTIME_EXCEPT("In \"" + args + "\", it have no values.") ;
            }
            auto val_last = val_area.find_last_not_of(" ") ;

            auto key = extract_single_arg(args.substr(0, eq_pos)) ;
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

        mode::Mode parse_mode_prefix(const std::string& prefix) {
            using namespace vind::mode ;
            static std::unordered_map<std::string, Mode> to_mode {
                {"gn", Mode::Normal},
                {"gv", Mode::Visual},
                {"en", Mode::EdiNormal},
                {"ev", Mode::EdiVisual},
                {"i",  Mode::Insert}
            } ;
            try {
                return to_mode.at(prefix) ;
            }
            catch(const std::out_of_range&) {
                return Mode::None ;
            }
        }

        RunCommandsIndex parse_run_command(const std::string& strcmd) {
            static std::unordered_map<std::string, RunCommandsIndex> parser {
                {"se",         RunCommandsIndex::SET},
                {"set",         RunCommandsIndex::SET},

                {"gnnoremap",   RunCommandsIndex::NOREMAP_GN},
                {"gnnorema",    RunCommandsIndex::NOREMAP_GN},
                {"gnnorem",     RunCommandsIndex::NOREMAP_GN},
                {"gnnore",      RunCommandsIndex::NOREMAP_GN},
                {"gnnor",       RunCommandsIndex::NOREMAP_GN},
                {"gnno",        RunCommandsIndex::NOREMAP_GN},
                {"gnn",         RunCommandsIndex::NOREMAP_GN},

                {"gvnoremap",   RunCommandsIndex::NOREMAP_GV},
                {"gvnorema",    RunCommandsIndex::NOREMAP_GV},
                {"gvnorem",     RunCommandsIndex::NOREMAP_GV},
                {"gvnore",      RunCommandsIndex::NOREMAP_GV},
                {"gvnor",       RunCommandsIndex::NOREMAP_GV},
                {"gvno",        RunCommandsIndex::NOREMAP_GV},
                {"gvn",         RunCommandsIndex::NOREMAP_GV},

                {"ennoremap",   RunCommandsIndex::NOREMAP_EN},
                {"ennorema",    RunCommandsIndex::NOREMAP_EN},
                {"ennorem",     RunCommandsIndex::NOREMAP_EN},
                {"ennore",      RunCommandsIndex::NOREMAP_EN},
                {"ennor",       RunCommandsIndex::NOREMAP_EN},
                {"enno",        RunCommandsIndex::NOREMAP_EN},
                {"enn",         RunCommandsIndex::NOREMAP_EN},

                {"evnoremap",   RunCommandsIndex::NOREMAP_EV},
                {"evnorema",    RunCommandsIndex::NOREMAP_EV},
                {"evnorem",     RunCommandsIndex::NOREMAP_EV},
                {"evnore",      RunCommandsIndex::NOREMAP_EV},
                {"evnor",       RunCommandsIndex::NOREMAP_EV},
                {"evno",        RunCommandsIndex::NOREMAP_EV},
                {"evn",         RunCommandsIndex::NOREMAP_EV},

                {"inoremap",    RunCommandsIndex::NOREMAP_IN},
                {"inorema",     RunCommandsIndex::NOREMAP_IN},
                {"inorem",      RunCommandsIndex::NOREMAP_IN},
                {"inore",       RunCommandsIndex::NOREMAP_IN},
                {"inor",        RunCommandsIndex::NOREMAP_IN},
                {"ino",         RunCommandsIndex::NOREMAP_IN},


                {"gnunmap",     RunCommandsIndex::UNMAP_GN},
                {"gnunma",      RunCommandsIndex::UNMAP_GN},
                {"gnunm",       RunCommandsIndex::UNMAP_GN},
                {"gnun",        RunCommandsIndex::UNMAP_GN},

                {"gvunmap",     RunCommandsIndex::UNMAP_GV},
                {"gvunma",      RunCommandsIndex::UNMAP_GV},
                {"gvunm",       RunCommandsIndex::UNMAP_GV},
                {"gvun",        RunCommandsIndex::UNMAP_GV},

                {"enunmap",     RunCommandsIndex::UNMAP_EN},
                {"enunma",      RunCommandsIndex::UNMAP_EN},
                {"enunm",       RunCommandsIndex::UNMAP_EN},
                {"enun",        RunCommandsIndex::UNMAP_EN},

                {"evunmap",     RunCommandsIndex::UNMAP_EV},
                {"evunma",      RunCommandsIndex::UNMAP_EV},
                {"evunm",       RunCommandsIndex::UNMAP_EV},
                {"evun",        RunCommandsIndex::UNMAP_EV},

                {"iunmap",      RunCommandsIndex::UNMAP_IN},
                {"iunma",       RunCommandsIndex::UNMAP_IN},
                {"iunm",        RunCommandsIndex::UNMAP_IN},
                {"iun",         RunCommandsIndex::UNMAP_IN},
                {"iu",          RunCommandsIndex::UNMAP_IN},


                {"gnmapclear",  RunCommandsIndex::MAPCLEAR_GN},
                {"gnmapclea",   RunCommandsIndex::MAPCLEAR_GN},
                {"gnmapcle",    RunCommandsIndex::MAPCLEAR_GN},
                {"gnmapcl",     RunCommandsIndex::MAPCLEAR_GN},
                {"gnmapc",      RunCommandsIndex::MAPCLEAR_GN},

                {"gvmapclear",  RunCommandsIndex::MAPCLEAR_GV},
                {"gvmapclea",   RunCommandsIndex::MAPCLEAR_GV},
                {"gvmapcle",    RunCommandsIndex::MAPCLEAR_GV},
                {"gvmapcl",     RunCommandsIndex::MAPCLEAR_GV},
                {"gvmapc",      RunCommandsIndex::MAPCLEAR_GV},

                {"enmapclear",  RunCommandsIndex::MAPCLEAR_EN},
                {"enmapclea",   RunCommandsIndex::MAPCLEAR_EN},
                {"enmapcle",    RunCommandsIndex::MAPCLEAR_EN},
                {"enmapcl",     RunCommandsIndex::MAPCLEAR_EN},
                {"enmapc",      RunCommandsIndex::MAPCLEAR_EN},

                {"evmapclear",  RunCommandsIndex::MAPCLEAR_EV},
                {"evmapclea",   RunCommandsIndex::MAPCLEAR_EV},
                {"evmapcle",    RunCommandsIndex::MAPCLEAR_EV},
                {"evmapcl",     RunCommandsIndex::MAPCLEAR_EV},
                {"evmapc",      RunCommandsIndex::MAPCLEAR_EV},

                {"imapclear",   RunCommandsIndex::MAPCLEAR_IN},
                {"imapclea",    RunCommandsIndex::MAPCLEAR_IN},
                {"imapcle",     RunCommandsIndex::MAPCLEAR_IN},
                {"imapcl",      RunCommandsIndex::MAPCLEAR_IN},
                {"imapc",       RunCommandsIndex::MAPCLEAR_IN},


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
                {"comc",        RunCommandsIndex::COMCLEAR}
            } ;

            try {
                return parser.at(strcmd) ;
            }
            catch(const std::out_of_range&) {
                return RunCommandsIndex::UNDEFINED ;
            }
        }
    }
}
