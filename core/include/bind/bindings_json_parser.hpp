#ifndef _BINDINGS_JSON_PARSER_HPP
#define _BINDINGS_JSON_PARSER_HPP

#include "bind/base/binded_func.hpp"
#include "bind/logger_parser.hpp"


namespace vind
{
    namespace bindjsonparser {
        using LoggerParserList = std::vector<LoggerParser::SPtr> ;

        void load_bindings_as_parser(
                const std::string& filepath,
                const std::vector<BindedFunc::SPtr>& all_func_list,
                std::array<LoggerParserList, mode::mode_num()>& mode_parser_list) ;
    }
}

#endif
