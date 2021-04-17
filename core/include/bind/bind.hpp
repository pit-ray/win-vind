#ifndef _BIND_HPP
#define _BIND_HPP

#include "bind/base/binded_func.hpp"
#include "bind/base/mode.hpp"
#include "bind/logger_parser.hpp"

namespace vind
{
    class KeyLoggerBase ;

    namespace keybind
    {
        void initialize() ;

        const BindedFunc::SPtr find_func_byname(const std::string& name) ;

        const LoggerParser::SPtr find_parser(
                const KeyLog& log,
                const LoggerParser::SPtr& low_priority_parser=nullptr,
                mode::Mode mode=mode::get_global_mode()) ;

        void undo_parsers(std::size_t n, mode::Mode mode=mode::get_global_mode()) ;

        void reset_parsers(mode::Mode mode=mode::get_global_mode()) ;

        void load_config() ;
    }
}

#endif
