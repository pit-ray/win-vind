#include "bind/bindings_json_parser.hpp"

#include <exception>
#include <memory>
#include <string>
#include <fstream>
#include <vector>

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

namespace
{
    using namespace vind ;

    bool setup_matcher_with_links(
            const BindedFunc::SPtr& func,
            const nlohmann::json& funcobj,
            const std::string& filepath) {
        //filepath is used only to print error message

        constexpr auto lcx_mode_num = static_cast<unsigned char>(mode::Mode::NUM) ;

        std::array<BindingsMatcher::SPtr, lcx_mode_num> matcher_list ;
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
                const std::vector<BindedFunc::SPtr>& all_func_list,
                std::vector<BindedFunc::SPtr>& loaded_func_list) {
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
            std::unordered_map<std::string, BindedFunc::SPtr> name2func ;
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

namespace vind
{

    namespace bindjsonparser {
        void setup_logger_parser_with_links(
                const BindedFunc::SPtr& func,
                const nlohmann::json& funcobj,
                std::array<LoggerParserList, mode::mode_num()>& mode_parser_list,
                const std::string& filepath) { //filepath is used only to print error message

            std::array<LoggerParser::SPtr, mode::mode_num()> mode_parser ;
            mode_parser.fill(nullptr) ;

            //if JSON's data is "edin": ["<guin>"], index_links[edin-index] = guin-index
            std::array<mode::Mode, mode::mode_num()> mode_links ;
            mode_links.fill(mode::Mode::None) ;

            for(std::size_t m_idx = 0 ; m_idx < mode::mode_num() ; m_idx ++) {

                auto mode_strcode = mode::get_mode_strcode(m_idx) ;
                if(mode_strcode.empty()) {
                    continue ;
                }

                try {
                    const auto& cmds = funcobj.at(mode_strcode) ;
                    if(!cmds.is_array()) {
                        PRINT_ERROR("The command lists should be array (" + func->name() + "/" + mode_strcode + ").") ;
                        continue ;
                    }
                    if(cmds.empty()) {
                        continue ;
                    }

                    if(cmds.size() == 1) {
                        auto str = cmds.front().get<std::string>() ;
                        if(str.front() == '<' && str.back() == '>') {
                            auto inside = util::A2a(str.substr(1, str.size() - 2)) ;
                            auto mode = mode::get_mode_from_strcode(inside) ;
                            if(mode != mode::Mode::None) {
                                mode_links[m_idx] = mode ;
                                continue ;
                            }
                        }
                    }

                    auto lgrparser = std::make_shared<LoggerParser>(func) ;
                    try {
                        lgrparser->append_binding_list(cmds.get<std::vector<std::string>>()) ;
                    }
                    catch(const std::runtime_error& e) {
                        PRINT_ERROR(func->name() + "/" + mode_strcode + " in " + filepath + " " + e.what()) ;
                        continue ;
                    }

                    if(lgrparser->has_bindings()) {
                        mode_parser[m_idx] = std::move(lgrparser) ;
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
            for(std::size_t m_idx = 0 ; m_idx < mode::mode_num() ; m_idx ++) {
                const auto link_mode = mode_links[m_idx] ;

                if(link_mode != mode::Mode::None && mode_parser[static_cast<std::size_t>(link_mode)]) {
                    mode_parser_list[m_idx].push_back(mode_parser[static_cast<std::size_t>(link_mode)]) ;
                }
                else if(mode_parser[m_idx]) {
                    mode_parser_list[m_idx].push_back(mode_parser[m_idx]) ;
                }
            }
        }


        void load_bindings_as_parser(
                const std::string& filepath,
                const std::vector<BindedFunc::SPtr>& all_func_list,
                std::array<LoggerParserList, mode::mode_num()>& mode_parser_list) {

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

            for(auto& v : mode_parser_list) {
                v.clear() ;
            }

            //create name lists of BindidFunc
            std::unordered_map<std::string, BindedFunc::SPtr> name2func ;
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
                    setup_logger_parser_with_links(func, obj, mode_parser_list, filepath) ;
                }
                catch(const std::out_of_range& e) {
                    PRINT_ERROR(std::string(e.what()) + ". The following syntax is invalid." + obj.dump()) ;
                    continue ;
                }
            }
        }

    }
}
