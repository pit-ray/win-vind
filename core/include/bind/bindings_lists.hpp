#ifndef _BINDINGS_LISTS_HPP
#define _BINDINGS_LISTS_HPP

#include "bind/base/binded_func.hpp"

#include <vector>

namespace vind
{
    namespace bindingslists {
        const std::vector<BindedFunc::SPtr>& get() ;
    }
}
#endif
