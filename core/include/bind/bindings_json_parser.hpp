#ifndef _BINDINGS_JSON_PARSER_HPP
#define _BINDINGS_JSON_PARSER_HPP

#include "bind/base/binded_func.hpp"


namespace vind
{
    namespace bindjsonparser {
        void load_bindings_json(
                const std::string& filepath,
                const std::vector<BindedFunc::shp_t>& all_func_list,
                std::vector<BindedFunc::shp_t>& loaded_func_list) ;
    }
}

#endif
