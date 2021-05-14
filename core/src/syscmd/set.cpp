#include "syscmd/set.hpp"

#include <iostream>
#include <string>

#include "g_params.hpp"

#include "util/def.hpp"
#include "util/string.hpp"


namespace
{
    inline std::string extract_valid_name_area(const std::string& str) {
        auto name_first = str.find_first_not_of(" ") ;
        if(name_first == std::string::npos) {
            throw RUNTIME_EXCEPT(":set has no option name.") ;
        }
        auto name_last = str.find_last_not_of(" ") ;
        auto name = str.substr(name_first, name_last - name_first + 1) ;

        if(name.find(" ") != std::string::npos) {
            throw RUNTIME_EXCEPT(":set has a invalid option name.") ;
        }

        return vind::util::A2a(name) ;
    }
}


namespace vind
{
    namespace syscmd {
        void do_set(const std::string& args) {
            if(args.empty()) {
                throw RUNTIME_EXCEPT(":set has no arguments.") ;
            }

            auto eq_pos = args.find_first_of("=") ;

            if(eq_pos == std::string::npos) { // bool
                auto name = extract_valid_name_area(args) ;
                if(name.size() > 2 && name[0] == 'n' && name[1] == 'o') {
                    gparams::set(name.substr(2), false) ;
                }
                else {
                    gparams::set(name, true) ;
                }
            }
            else { //string, int, bool
                // extract name
                auto name_area = util::A2a(args.substr(0, eq_pos)) ;
                auto name = extract_valid_name_area(name_area) ;

                // extract value
                auto value_area = args.substr(eq_pos + 1) ;
                auto value_first = value_area.find_first_not_of(" ") ;
                if(value_first == std::string::npos) {
                    throw RUNTIME_EXCEPT("There is no value in :set {name}" \
                            + args.substr(eq_pos, 1) + "{value} syntax.") ;
                }
                auto value_last = value_area.find_last_not_of(" ") ;
                auto value = value_area.substr(value_first, value_last - value_first + 1) ;

                if(value.find_first_not_of("0123456789.") == std::string::npos) {
                    gparams::set(name, std::stod(value)) ;
                }
                else {
                    gparams::set(name, value) ;
                }
            }
        }
    }
}
