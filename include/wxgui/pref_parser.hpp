#ifndef _PREF_PARSER_HPP
#define _PREF_PARSER_HPP

#include <unordered_map>
#include <string>

namespace PrefParser
{
    using ums_str_t = std::unordered_map<std::string, std::string> ;
    using ums_bool_t = std::unordered_map<std::string, bool> ;

    using ums_ums_str_t = std::unordered_map<std::string, ums_str_t> ;

    const ums_ums_str_t load_binds() ;
    const ums_ums_str_t& load_default_binds() ;
    void save_binds(const ums_ums_str_t& maps) ;

    const ums_str_t load_params() ;
    const ums_str_t& load_default_params() ;
    void save_params(const ums_str_t& params) ;

    const ums_str_t load_opts_params() ;
    const ums_str_t& load_default_opts_params() ;
    void save_opts_params(const ums_str_t& params) ;

    const ums_bool_t load_opts_bool() ;
    const ums_bool_t& load_default_opts_bool() ;
    void save_opts_bool(const ums_bool_t& opts) ;

    const ums_str_t load_exapps() ;
    const ums_str_t& load_default_exapps() ;
    void save_exapps(const ums_str_t& exapps) ;

    const std::string load_kbtype() ;
    const std::string load_default_kbtype() ;
    void save_kbtype(const std::string& path) ;
    void save_kbtype(std::string&& path) ;

    const std::string load_uilang() ;
    const std::string load_default_uilang() ;
    void save_uilang(const std::string& path) ;
    void save_uilang(std::string&& path) ;

    const std::string load_sticon() ;
    const std::string load_default_sticon() ;
    void save_sticon(const std::string& path) ;
    void save_sticon(std::string&& path) ;
}

#endif