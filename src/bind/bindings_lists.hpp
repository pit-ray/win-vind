#ifndef _BINDINGS_LISTS_HPP
#define _BINDINGS_LISTS_HPP

#include "binded_func.hpp"

#include <vector>

namespace vind
{
    namespace bind
    {
        const std::vector<BindedFunc::SPtr>& all_global_binded_funcs() ;
    }
}
#endif
