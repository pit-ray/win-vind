#include "bind/bindings_json_parser.hpp"

#include <exception>
#include <string>
#include <fstream>
#include <vector>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "coreio/path.hpp"
#include "coreio/err_logger.hpp"

#include "util/def.hpp"

#include "bind/base/binded_func.hpp"
#include "bind/bindings_parser.hpp"
#include "bind/base/mode.hpp"

namespace
{
    using namespace vind ;

    bool setup_matcher_with_links(
            const BindedFunc::shp_t& func,
            const nlohmann::json& funcobj,
            const std::string& filepath) {
        //filepath is used only to print error message

        constexpr auto lcx_mode_num = static_cast<unsigned char>(mode::Mode::NUM) ;

        std::array<BindingsMatcher::shp_t, lcx_mode_num> matcher_list ;
        matcher_list.fill(nullptr) ;

        //if JSON's data is "edin": ["<guin>"], index_links[edin-index] = guin-index
        constexpr auto lcx_uchar_none_mode = static_cast<unsigned char>(mode::Mode::None) ;
        std::array<unsigned char, lcx_mode_num> index_links ;
        index_links.fill(lcx_uchar_none_mode) ;

        for(unsigned char m_idx = 0 ; m_idx < lcx_mode_num ; m_idx ++) {
            auto mode_strcode = mode::get_mode_strcode(m_idx) ;
            if(mode_strcode.empty()) {
                continue ;
            }

            func->register_matcher(m_idx, nullptr) ;

            try {
                const auto& cmds = funcobj.at(mode_strcode) ;
                if(!cmds.is_array()) {
                    PRINT_ERROR("The command lists should be array (" + func->name() + "/" + mode_strcode + ").") ;
                    continue ;
                }
                if(cmds.empty()) {
                    continue ;
                }

                BindingsMatcher::cmdlist_t cmdlist ;
                for(std::string cmdstr : cmds) {
                    if(cmdstr.empty()) {
                        continue ;
                    }

                    BindingsMatcher::cmd_t cmd ;
                    try {
                        auto mode = bindparser::parse_bindings(cmd, cmdstr) ;
                        if(mode != mode::Mode::None) {
                            index_links[m_idx] = static_cast<unsigned char>(mode) ;
                            cmdlist.clear() ;
                            break ;
                        }
                    }
                    catch(const std::runtime_error& e) {
                        PRINT_ERROR(func->name() + "/" + mode_strcode + " in " + filepath + " " + e.what()) ;
                        continue ;
                    }

                    cmdlist.push_back(std::move(cmd)) ;
                }

                if(!cmdlist.empty()) {
                    //create BindingsMatcher for one mode
                    matcher_list[m_idx] = std::make_shared<BindingsMatcher>(std::move(cmdlist)) ;
                }
            }
            catch(const std::out_of_range&) {
                continue ;
            }
        }

        //If there are some key-bindings fields of the mode having <mode-name> (e.q. <guin>, <edin>) in bindings.json ,
        //they are copied key-bindings from the first mode in json-array to them.
        //Ex) "guin": ["<Esc>", "happy"]
        //    "edin": ["<guin>", "<guii>"]    -> same as "guin"'s key-bindings(<Esc>, "happy")
        bool having_non_null = false ;
        for(unsigned char m_idx = 0 ; m_idx < lcx_mode_num ; m_idx ++) {
            const auto link_idx = index_links[m_idx] ;

            if(link_idx != lcx_uchar_none_mode) {
                matcher_list[m_idx] = matcher_list.at(link_idx) ;
            }

            if(matcher_list[m_idx] != nullptr) {
                having_non_null = true ;
            }
        }

        if(!having_non_null) {
            return false ;
        }

        for(unsigned char m_idx = 0 ; m_idx < lcx_mode_num ; m_idx ++) {
            func->register_matcher(m_idx, matcher_list[m_idx]) ;
        }
        return true ;
    }
}

namespace vind
{
    namespace bindjsonparser {
        void load_bindings_json(
                const std::string& filepath,
                const std::vector<BindedFunc::shp_t>& all_func_list,
                std::vector<BindedFunc::shp_t>& loaded_func_list) {
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

            loaded_func_list.clear() ;

            //create name lists of BindidFunc
            std::unordered_map<std::string, BindedFunc::shp_t> name2func ;
            for(const auto& func : all_func_list) {
                name2func[func->name()] = func ;
            }

            for(const auto& obj : json) {
                if(!obj.is_object()) {
                    PRINT_ERROR("The child of root-array should be object. (" + filepath +").\t" + obj.dump()) ;
                    continue ;
                }

                try {
                    auto& func = name2func.at(obj.at("name")) ;

                    if(setup_matcher_with_links(func, obj, filepath)) {
                        loaded_func_list.push_back(func) ;
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
