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

    const ums_bool_t load_opts() ;
    const ums_bool_t& load_default_opts() ;
    void save_opts(const ums_bool_t& opts) ;
}

#endif