#include "syscmd/map.hpp"

#include "mode.hpp"
#include "util/def.hpp"
#include <unordered_map>

namespace vind
{
    namespace syscmd {

        mode::Mode parse_mode_prefix(const std::string& prefix) {
            using namespace vind::mode ;
            static std::unordered_map<std::string, Mode> to_mode {
                {"gn", Mode::Normal},
                {"gv", Mode::Visual},
                {"en", Mode::EdiNormal},
                {"ev", Mode::EdiVisual},
                {"i", Mode::Insert}
            } ;
            return to_mode.at(prefix) ;
        }


        void do_noremap(mode::Mode mode, const std::string& args) {
            if(args.empty()) {
                throw RUNTIME_EXCEPT("The argument is empty in noremap of " + mode::get_mode_strcode(mode)) ;
            }

            auto src_cmd_first = args.find_first_not_of(" ") ;
            if(src_cmd_first == std::string::npos) {
                throw RUNTIME_EXCEPT("noremap does not have the first argument") ;
            }
            auto delimitator_space = args.find_first_of(" ", src_cmd_first + 1) ;
            if(delimitator_space == std::string::npos) {
                throw RUNTIME_EXCEPT("noremap does not have the second argument, with space delimitator") ;
            }
            auto src_cmd = args.substr(src_cmd_first, delimitator - src_cmd_first) ;

            auto target_first = args.find_first_not_of(" ", delimitator_space + 1) ;
            if(target_first == std::string::npos) {
                // Does not have second argument
            }
            auto target_last = args.find_last_not_of(" ") ;
            auto target = args.substr(target_first, target_last - target_first + 1) ;
            if(target.find(" ") != std::string::npos) {
                // Over argument
            }

            // mode, src_cmd, target
        }

        void do_unmap(mode::Mode mode, const std::string& args) {

            if(args.empty()) {
                // does not have argument is empty
            }

            auto first = args.find_first_not_of(" ") ;
            if(first == std::string::pos) {
                // does not have argument
            }
            auto last = args.find_last_not_of(" ") ;
            auto cmd = args.substr(first, last - first + 1) ;

            if(cmd.find(" ") != std::string::npos) {
            }

            // 
        }

        void do_mapclear(mode::Mode mode) {
            // clear
        }

    }
}
