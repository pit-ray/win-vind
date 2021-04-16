#include "bind/bindings_json_parser.hpp"

#include <exception>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "bind/logger_parser.hpp"
#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "coreio/path.hpp"
#include "coreio/err_logger.hpp"

#include "util/def.hpp"
#include "util/string.hpp"

#include "bind/base/binded_func.hpp"
#include "bind/bindings_parser.hpp"
#include "bind/base/mode.hpp"

namespace vind
{
    namespace bindjsonparser {
        void parse_bindings_as_list(
                const std::string& filepath,
                const std::vector<BindedFunc::SPtr>& all_func_list,
                ParsedStringBindingLists& parsed_bindings) {

            std::ifstream ifs(path::to_u8path(filepath)) ;
            nlohmann::json json ;
            ifs >> json ;

            if(json.empty()) {
                throw RUNTIME_EXCEPT("Could not load \"" + filepath + "\".") ;
            }

            if(!json.is_array()) {
                throw RUNTIME_EXCEPT("The root element of \"" + filepath + "\" should be array.") ;
            }

            if(all_func_list.empty()) {
                throw LOGIC_EXCEPT("No BindedFunc is defined.") ;
            }

            //clear by swapping
            ParsedStringBindingLists().swap(parsed_bindings) ;

            for(const auto& obj : json) {
                if(!obj.is_object()) {
                    PRINT_ERROR("The child of root-array should be object. (" + filepath +").\t" + obj.dump()) ;
                    continue ;
                }

                try {
                    auto& mode_array = parsed_bindings[obj.at("name").get<std::string>()] ;

                    for(std::size_t m_idx = 0 ; m_idx < mode::mode_num() ; m_idx ++) {
                        auto mode_strcode = mode::get_mode_strcode(m_idx) ;
                        if(mode_strcode.empty()) {
                            continue ;
                        }

                        try {
                            const auto& cmds = obj.at(mode_strcode) ;
                            if(!cmds.is_array()) {
                                PRINT_ERROR("The command lists should be array (" + \
                                        obj.at("name").get<std::string>() + "/" + mode_strcode + ").") ;
                                continue ;
                            }
                            if(cmds.empty()) {
                                continue ;
                            }

                            try {
                                mode_array[m_idx] = cmds.get<std::vector<std::string>>() ;
                            }
                            catch(const std::runtime_error& e) {
                                std::stringstream ss ;
                                ss << "Could not parse array of {" ;
                                ss << "name: \"" << obj.at("name").get<std::string>() << "\", " ;
                                ss << "\"" + mode_strcode + "\"}" + " in " + filepath + ". " + e.what() ;
                                PRINT_ERROR(ss.str()) ;
                                continue ;
                            }
                        }
                        catch(const std::out_of_range&) {
                            continue ;
                        }
                    }
                }
                catch(const std::out_of_range& e) {
                    PRINT_ERROR(std::string(e.what()) + ". The following syntax is invalid." + obj.dump()) ;
                    continue ;
                }
            }
        }
    }
}
