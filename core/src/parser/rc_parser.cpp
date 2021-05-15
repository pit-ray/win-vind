#include "parser/rc_parser.hpp"

#include "util/def.hpp"

#include <string>
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
                throw RUNTIME_EXCEPT("\"" + line + "\" has no arguments.") ;
            }

            auto args_first = line.find_first_not_of(" ", cmd_last + 1) ;
            if(args_first == std::string::npos) {
                throw RUNTIME_EXCEPT("\"" + line + "\" is invalid syntax.") ;
            }
            auto args_last = line.find_last_not_of(" ") ;

            auto cmd = line.substr(cmd_first, cmd_last - cmd_first + 1) ;
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

            auto arg1 = args.substr(arg1_first, delimiter - arg1_first) ;
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
    }
}
