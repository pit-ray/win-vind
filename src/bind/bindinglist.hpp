#ifndef _BINDINGS_LISTS_HPP
#define _BINDINGS_LISTS_HPP

#include "bindedfunc.hpp"

#include <vector>

namespace vind
{
    namespace bind
    {
        const std::vector<BindedFunc::SPtr>& all_global_binded_funcs() ;

        BindedFunc::SPtr search_func(std::size_t id) ;
        BindedFunc::SPtr search_func(const std::string& name) ;

        inline bool check_if_func(std::size_t id) {
            return search_func(id) != nullptr ;
        }
    }
}
#endif
